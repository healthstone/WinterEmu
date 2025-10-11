#include "CharHandlers.hpp"
#include "utils/utf8utils/UTF8Utils.hpp"
#include "utils/util/Util.hpp"
#include "src/game/enums/Classes.hpp"
#include "src/game/enums/CharacterEnums.hpp"
#include "src/game/enums/DBCStructure.hpp"
#include "src/game/enums/Gender.hpp"
#include "src/game/enums/PetDefines.hpp"
#include "src/game/enums/PlayerEnums.hpp"
#include "src/game/enums/ItemDefines.hpp"
#include "src/game/Entity/PlayerInfo/PlayerInfo.hpp"
#include "utils/RealmStringValidator.hpp"

#define MAX_PLAYER_NAME          12                         // max allowed by client name length

boost::asio::awaitable<void>
CharHandlers::handleCharacterEnum(std::shared_ptr<GameSession> session) {
    Logger::get()->debug("CharHandlers::handleCharacterEnum - CMSG_CHAR_ENUM");
    auto dbcMgr = session->server()->getDBCMgr();
    session->resetLegitCharacterForAccount();

    WoWPacket pkt(WoWOpcodes::SMSG_CHAR_ENUM);

    // Получаем данные из базы
    auto characters = co_await fetchFromDB(session);
    if (!characters.empty()) {
        pkt.write_uint8(static_cast<uint8_t>(characters.size())); // Количество персонажей

        for (const auto &character: characters) {
            ObjectGuid playerGuid = ObjectGuid::Create<HighGuid::Player>(character.m_guid);
            session->addLegitCharacterForAccount(playerGuid, character);

            if (!session->isCanCreateDK() && character.m_level >= 55)
                session->setIsCanCreateDK(true);

            if (static_cast<Classes>(character.m_class) == Classes::CLASS_DEATH_KNIGHT) {
                session->addDKCountOnRealm();
            }

            if (session->getAccountTeam() == Team::TEAM_OTHER) {
                session->setAccountTeam(dbcMgr->teamForRace(character.m_race));
            }

            // GUID
            pkt.write_uint64_le(playerGuid.GetRawValue());

            // Основная информация
            pkt.write_string_nt_be(character.m_name);
            pkt.write_uint8(character.m_race);
            pkt.write_uint8(character.m_class);
            pkt.write_uint8(character.m_gender);
            pkt.write_uint8(character.m_skin);
            pkt.write_uint8(character.m_face);
            pkt.write_uint8(character.m_hairStyle);
            pkt.write_uint8(character.m_hairColor);
            pkt.write_uint8(character.m_facialStyle);

            pkt.write_uint8(character.m_level);           // level
            pkt.write_uint32_le(character.m_zone);        // Zone
            pkt.write_uint32_le(character.m_map);         // Map

            pkt.write_float_le(character.m_position_x);  // X
            pkt.write_float_le(character.m_position_y);  // Y
            pkt.write_float_le(character.m_position_z);  // Z

            // GuildID
            pkt.write_uint32_le(character.guildid.value_or(0));

            // CharFlags
            uint32_t charFlags = 0;
            uint32_t playerFlags = character.m_playerFlags;
            uint16_t atLoginFlags = character.m_at_login;

            if (atLoginFlags & AT_LOGIN_RESURRECT)
                playerFlags &= ~PLAYER_FLAGS_GHOST;
            if (playerFlags & PLAYER_FLAGS_HIDE_HELM)
                charFlags |= CHARACTER_FLAG_HIDE_HELM;
            if (playerFlags & PLAYER_FLAGS_HIDE_CLOAK)
                charFlags |= CHARACTER_FLAG_HIDE_CLOAK;
            if (playerFlags & PLAYER_FLAGS_GHOST)
                charFlags |= CHARACTER_FLAG_GHOST;
            if (atLoginFlags & AT_LOGIN_RENAME)
                charFlags |= CHARACTER_FLAG_RENAME;
            if (character.banned_guid)
                charFlags |= CHARACTER_FLAG_LOCKED_BY_BILLING;

            pkt.write_uint32_le(charFlags);

            // character customize flags
            if (atLoginFlags & AT_LOGIN_CUSTOMIZE)
                pkt.write_uint32_le(CHAR_CUSTOMIZE_FLAG_CUSTOMIZE);
            else if (atLoginFlags & AT_LOGIN_CHANGE_FACTION)
                pkt.write_uint32_le(CHAR_CUSTOMIZE_FLAG_FACTION);
            else if (atLoginFlags & AT_LOGIN_CHANGE_RACE)
                pkt.write_uint32_le(CHAR_CUSTOMIZE_FLAG_RACE);
            else
                pkt.write_uint32_le(CHAR_CUSTOMIZE_FLAG_NONE);

            // First login
            pkt.write_uint8(atLoginFlags & AT_LOGIN_FIRST ? 1 : 0);

            // Pet info
            pkt.write_uint32_le(character.pet_modelid.value_or(0));
            pkt.write_uint32_le(character.pet_level.value_or(0));
            pkt.write_uint32_le(0); // Pet family

            auto itemTemplateMgr = session->server()->getItemTemplateMgr();
            std::vector<std::string_view> equipment = Util::Tokenize(character.m_equipmentCache, ' ', false);
            for (uint8_t slot = 0; slot < INVENTORY_SLOT_BAG_END; ++slot) {
                uint32_t const visualBase = slot * 2;
                std::optional<uint32_t> itemId;
                if (visualBase < equipment.size())
                    itemId = Util::stringToUInt32(equipment[visualBase]);

                ItemTemplate const* proto = nullptr;
                if (itemId)
                    proto = itemTemplateMgr->getItemTemplate(*itemId);

                if (!proto)
                {
                    if (!itemId || *itemId)
                    {
                        Logger::get()->error("Player {} has invalid equipment '{}' in `equipmentcache` at index {}. Skipped.",
                                playerGuid.ToString(), (visualBase < equipment.size()) ? std::string(equipment[visualBase]) : "<none>", visualBase);
                    }

                    pkt.write_uint32_le(0); // proto->DisplayInfoID
                    pkt.write_uint8(0);     // proto->InventoryType
                    pkt.write_uint32_le(0); // enchant ? enchant->ItemVisual : 0
                    continue;
                }

                SpellItemEnchantmentDBC const* enchant = nullptr;
                std::optional<uint32_t> enchants;

                if ((visualBase + 1) < equipment.size())
                    enchants = Util::stringToUInt32(equipment[visualBase + 1]);

                if (!enchants) {
                    Logger::get()->warn("Player {} has invalid enchantment info '{}' in `equipmentcache` at index {}. Skipped.",
                                        playerGuid.ToString(),
                                        ((visualBase + 1) < equipment.size()) ? std::string(equipment[visualBase + 1]) : "<none>", visualBase + 1);
                    enchants = 0;
                }

                for (uint8_t enchantSlot = PERM_ENCHANTMENT_SLOT; enchantSlot <= TEMP_ENCHANTMENT_SLOT; ++enchantSlot) {
                    uint32_t enchantId = 0x0000FFFF & ((*enchants) >> (enchantSlot * 16));
                    if (!enchantId)
                        continue;

                    enchant = dbcMgr->getSpellItemEnchantmentDBC(enchantId);
                    if (enchant)
                        break;
                }

                pkt.write_uint32_le(proto->DisplayInfoID);
                pkt.write_uint8(proto->InventoryType);
                pkt.write_uint32_le(enchant ? enchant->ItemVisual : 0);
            }
        }
    } else {
        pkt.write_uint8(0); // Количество персонажей (0)
    }

    session->send_packet(std::make_shared<WoWPacket>(pkt));
}

boost::asio::awaitable<void>
CharHandlers::handleCharacterCreate(std::shared_ptr<GameSession> session, const std::shared_ptr<WoWPacket> &p) {
    auto log = Logger::get();
    auto ccd = ReadPacketFields(p);
    if (!ccd) {
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_ERROR);
        co_return;
    }

    auto dbcMgr = session->server()->getDBCMgr();
    uint32_t m_expansion = session->getAccount()->expansion ? session->getAccount()->expansion.value()
                                                            : 2; // default WoTLK addon = 2
    std::string m_accountName = session->getAccount()->username.value();

    ChrRacesDBC const *raceEntry = dbcMgr->getChrRacesDBC(ccd->m_race);
    if (!raceEntry) {
        log->error(
                "CharHandlers::handleCharacterCreate: Race ({}) not found in DBC while creating new char for account:[{}] wrong DBC files or cheater?",
                ccd->m_race, m_accountName);
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_FAILED);
        co_return;
    }

    // prevent character creating Expansion race without Expansion account
    if (raceEntry->RequiredExpansion > m_expansion) {
        log->error(
                "CharHandlers::handleCharacterCreate: Expansion {} account:[{}] tried to Create character with expansion {} race ({})",
                m_expansion, m_accountName, raceEntry->RequiredExpansion, ccd->m_race);
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_EXPANSION);
        co_return;
    }

    // TC RBAC_PERM_SKIP_CHECK_CHARACTER_CREATION_RACEMASK
    if (raceEntry->Alliance == CHRRACES_ALLIANCE_TYPE_NOT_PLAYABLE || raceEntry->HasFlag(CHRRACES_FLAGS_NOT_PLAYABLE)) {
        log->error(
                "CharHandlers::handleCharacterCreate: Race ({}) was not playable but requested while creating new char for account:[{}] wrong DBC files or cheater?",
                ccd->m_race, m_accountName);
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_DISABLED);
        co_return;
    }

    //TC CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK
    uint32_t raceMaskDisabled = 0;
    if ((1 << (ccd->m_race - 1)) & raceMaskDisabled) {
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_DISABLED);
        co_return;
    }

    ChrClassesDBC const *classEntry = dbcMgr->getChrClassesDBC(ccd->m_class);
    if (!classEntry) {
        log->error(
                "CharHandlers::handleCharacterCreate: Class ({}) not found in DBC while creating new char for account:[{}] wrong DBC files or cheater?",
                ccd->m_class, m_accountName);
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_FAILED);
        co_return;
    }

    // prevent character creating Expansion class without Expansion account
    if (classEntry->RequiredExpansion > m_expansion) {
        log->error(
                "CharHandlers::handleCharacterCreate: Expansion {} account:[{}] tried to Create character with expansion {} class ({})",
                m_expansion, m_accountName, classEntry->RequiredExpansion, ccd->m_class);
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_EXPANSION_CLASS);
        co_return;
    }

    //TC CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK
    uint32_t classMaskDisabled = 0;
    if ((1 << (ccd->m_class - 1)) & classMaskDisabled) {
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_DISABLED);
        co_return;
    }

    if (static_cast<Classes>(ccd->m_class) == Classes::CLASS_DEATH_KNIGHT) {
        // level check >= 55lvl exists
        if (!session->isCanCreateDK()) {
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_LEVEL_REQUIREMENT);
            co_return;
        }

        // DK count check
        if (session->getDKCountOnRealm() > 0) {
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_UNIQUE_CLASS_LIMIT);
            co_return;
        }
    }

    if (session->getCharactersCountOnRealm() + 1 > MAX_CHARACTERS_PER_REALM) {
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_SERVER_LIMIT);
        co_return;
    }

    // prevent character creating with invalid name
    if (!UTF8Utils::normalizePlayerName(ccd->m_name)) {
        log->error("CharHandlers::handleCharacterCreate: Account:[{}] but tried to Create character with empty [name] ",
                   m_accountName);
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_NAME_NO_NAME);
        co_return;
    }

    ResponseCodes res = checkPlayerName(session, ccd->m_name);
    if (res != ResponseCodes::CHAR_NAME_SUCCESS) {
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, res);
        co_return;
    }

    auto usernameCount = co_await fetchUsernameCountFromDB(session, ccd->m_name);
    if (usernameCount > 0) {
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_NAME_IN_USE);
        co_return;
    }

    bool allowTwoSideAccounts = !session->server()->getRealm()->isPvPRealm();
    if (!allowTwoSideAccounts) {
        // Актуально только для последующих чаров (не первых), когда тима уже определена
        if (session->getAccountTeam() != Team::TEAM_OTHER) {
            if (session->getAccountTeam() != dbcMgr->teamForRace(ccd->m_race)) {
                sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_PVP_TEAMS_VIOLATION);
                co_return;
            }
        }
    }

    PlayerInfo const *info = session->server()->getPlayerInfoMgr()->getPlayerInfo(ccd->m_race, ccd->m_class);
    if (!info) {
        log->error("CharHandlers::handleCharacterCreate: Possible hacking attempt: "
                   "Account {} tried to create a character named '{}' with an invalid race/class pair ({}/{}) - refusing to do so.",
                   m_accountName, ccd->m_name, ccd->m_race, ccd->m_class);
        sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_RESTRICTED_RACECLASS);
        co_return;
    }

    co_await handleInsertCharacter(session, ccd.value(), info);
    co_await handleUpdateRealmCharacters(session, session->getCharactersCountOnRealm() + 1);

    sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_SUCCESS);
    co_return;
}

ResponseCodes CharHandlers::checkPlayerName(std::shared_ptr<GameSession> session, std::string_view name)
{
    std::wstring wname;
    if (!UTF8Utils::Utf8toWStr(name, wname))
        return ResponseCodes::CHAR_NAME_INVALID_CHARACTER;

    if (wname.size() > MAX_PLAYER_NAME)
        return ResponseCodes::CHAR_NAME_TOO_LONG;

    if (wname.size() < 2)
        return ResponseCodes::CHAR_NAME_TOO_SHORT;

    auto currentRealm = session->server()->getRealm();
    auto dbcMgr = session->server()->getDBCMgr();

    // TC CONFIG_STRICT_PLAYER_NAMES : default 0
    uint32_t strictMask = 0;
    if (!RealmStringValidator::isValidString(currentRealm->Zone, wname, strictMask, false, true))
        return ResponseCodes::CHAR_NAME_MIXED_LANGUAGES;

    UTF8Utils::wstrToLower(wname);
    for (size_t i = 2; i < wname.size(); ++i)
        if (wname[i] == wname[i-1] && wname[i] == wname[i-2])
            return ResponseCodes::CHAR_NAME_THREE_CONSECUTIVE;

    return dbcMgr->validateName(wname, session->getSessionDbcLocale());
}

void CharHandlers::sendCharResponse(std::shared_ptr<GameSession> session, WoWOpcodes opcode, ResponseCodes result) {
    WoWPacket pkt(opcode);
    pkt.write_uint8(static_cast<uint8_t>(result));
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}

std::optional<CharCreateData> CharHandlers::ReadPacketFields(const std::shared_ptr<WoWPacket> &p) {
    try {
        /// User specified variables
        CharCreateData ccd;
        ccd.m_name = p->read_string_nt_be();
        ccd.m_race = p->read_uint8();
        ccd.m_class = p->read_uint8();
        ccd.m_gender = p->read_uint8();
        ccd.m_skin = p->read_uint8();
        ccd.m_face = p->read_uint8();
        ccd.m_hairStyle = p->read_uint8();
        ccd.m_hairColor = p->read_uint8();
        ccd.m_facialHair = p->read_uint8();
        ccd.m_outfitId = p->read_uint8();

        Logger::get()->debug("Create character:\n"
                             "{} name\n"
                             "{} Race\n"
                             "{} Class\n"
                             "{} Gender\n"
                             "{} Skin\n"
                             "{} Face\n"
                             "{} HairStyle\n"
                             "{} HairColor\n"
                             "{} FacialHair\n"
                             "{} OutfitId", ccd.m_name, ccd.m_race, ccd.m_class, ccd.m_gender, ccd.m_skin, ccd.m_face,
                             ccd.m_hairStyle, ccd.m_hairColor, ccd.m_facialHair, ccd.m_outfitId);


        return ccd;

    } catch (const std::exception &ex) {
        Logger::get()->error("[CharHandlers::ReadPacketFields] DB exception: {}", ex.what());
        return std::nullopt;
    }
}

boost::asio::awaitable<std::vector<CharacterEnumRow>> CharHandlers::fetchFromDB(std::shared_ptr<GameSession> session) {
    try {
        PreparedStatement stmt("SELECT_CHAR_ENUM");
        stmt.set_param(0, PetSaveMode::PET_SAVE_AS_CURRENT);
        stmt.set_param(1, session->getAccount()->id);
        auto rows = co_await session->server()->db()->execute_async_many<CharacterEnumRow>(stmt);
        co_return rows;
    } catch (const std::exception &ex) {
        Logger::get()->error("[CharHandlers::fetchFromDB] DB exception: {}", ex.what());
        co_return std::vector<CharacterEnumRow>{};
    }
}

boost::asio::awaitable<uint64_t>
CharHandlers::fetchUsernameCountFromDB(std::shared_ptr<GameSession> session, const std::string &charName) {
    try {
        PreparedStatement stmt("SELECT_COUNT_CHARS_BY_USERNAME");
        stmt.set_param(0, charName);
        auto row = co_await session->server()->db()->execute_async_one<uint64_t>(stmt);
        co_return row.value();
    } catch (const std::exception &ex) {
        Logger::get()->error("[CharHandlers::fetchUsernameCountFromDB] DB exception: {}", ex.what());
        co_return 1;
    }
}

boost::asio::awaitable<void>
CharHandlers::handleInsertCharacter(std::shared_ptr<GameSession> session, const CharCreateData &ccd,
                                    PlayerInfo const *playerInfo) {
    try {
        PreparedStatement stmt("INSERT_CHARACTER");
        //conn.prepare("INSERT_CHARACTER",
        //                     //                                         1       2    3      4      5       6     7    8     9     10      11         12          13          14         15          16
        //                     fmt::format("INSERT INTO {}.characters (account, name, race, class, gender, level, xp, money, skin, face, hairstyle, haircolor, facialstyle, bankslots, reststate, playerflags, "
        //                                 //17       18              19              20          21          22          23         24      25        26       27        28
        //                                 "map, instance_id, instance_mode_mask, position_x, position_y, position_z, orientation, trans_x, trans_y, trans_z, trans_o, transguid, "
        //                                 // 29         30
        //                                 "taximask, cinematic, "
        //                                 //   31        32          33          34              35                 36                  37
        //                                 "totaltime, leveltime, rest_bonus, logout_time, is_logout_resting, resettalents_cost, resettalents_time, "
        //                                 //   38             39         40      41
        //                                 "extra_flags, stable_slots, at_login, zone, "
        //                                 //     42              43          44              45                46                47                 48
        //                                 "death_expire_time, taxi_path, arenapoints, totalhonorpoints, todayhonorpoints, yesterdayhonorpoints, totalkills, "
        //                                 //    49           50            51              52              53         54     55       56      57     58
        //                                 "todaykills, yesterdaykills, chosentitle, knowncurrencies, watchedfaction, drunk, health, power1, power2, power3, "
        //                                 // 59      60      61      62      63              64                65               66             67          68         69          70            71
        //                                 "power4, power5, power6, power7, latency, talentgroupscount, activetalentgroup, exploredzones, equipmentcache, ammoid, knowntitles, actionbars, grantablelevels) "
        //                                 "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $40, $41, $42, $43, $44, $45, $46, $47, $48, $49, $50, $51, $52, $53, $54, $55, $56, $57, $58, $59, $60, $61, $62, $63, $64, $65, $66, $67, $68, $69, $70, $71)",
        //                                 relay_schema));

        stmt.set_param(0, session->getAccount()->id);
        stmt.set_param(1, ccd.m_name);
        stmt.set_param(2, ccd.m_race);
        stmt.set_param(3, ccd.m_class);
        stmt.set_param(4, ccd.m_gender);
        stmt.set_param(5, 1);   // level
        stmt.set_param(6, 0);   // exp
        stmt.set_param(7, 0);   // money
        stmt.set_param(8, ccd.m_skin);   // skin
        stmt.set_param(9, ccd.m_face);   // face
        stmt.set_param(10, ccd.m_hairStyle);   // hairstyle
        stmt.set_param(11, ccd.m_hairColor);   // haircolor
        stmt.set_param(12, ccd.m_facialHair);   // facialstyle
        stmt.set_param(13, 0);   // bankslots
        stmt.set_param(14, 0);   // reststate
        stmt.set_param(15, 0);   // playerflags

        stmt.set_param(16, playerInfo->mapId);        // map
        stmt.set_param(17, 0);                        // instance_id
        stmt.set_param(18, 0);                        // instance_mode_mask
        stmt.set_param(19, playerInfo->positionX);    // position_x
        stmt.set_param(20, playerInfo->positionY);    // position_y
        stmt.set_param(21, playerInfo->positionZ);    // position_z
        stmt.set_param(22, playerInfo->orientation);  // orientation
        stmt.set_param(23, 0);                        // trans_x
        stmt.set_param(24, 0);                        // trans_y
        stmt.set_param(25, 0);                        // trans_z
        stmt.set_param(26, 0);                        // trans_o
        stmt.set_param(27, 0);                        // transguid

        stmt.set_param(28, "");                       // taximask
        stmt.set_param(29, 0);                        // cinematic

        stmt.set_param(30, 0);                        // totaltime
        stmt.set_param(31, 0);                        // leveltime
        stmt.set_param(32, 0);                        // rest_bonus
        stmt.set_param(33, 0);                        // logout_time
        stmt.set_param(34, 0);                        // is_logout_resting
        stmt.set_param(35, 0);                        // resettalents_cost
        stmt.set_param(36, 0);                        // resettalents_time

        stmt.set_param(37, 0);                        // extra_flags
        stmt.set_param(38, 0);                        // stable_slots

        uint8_t m_atLoginFlags = AT_LOGIN_NONE;
        m_atLoginFlags |= AT_LOGIN_FIRST;
        stmt.set_param(39, m_atLoginFlags);           // at_login
        stmt.set_param(40, 0);                        // zone

        stmt.set_param(41, 0);                        // death_expire_time
        stmt.set_param(42, "");                       // taxi_path
        stmt.set_param(43, 0);                        // arenapoints
        stmt.set_param(44, 0);                        // totalhonorpoints
        stmt.set_param(45, 0);                        // todayhonorpoints
        stmt.set_param(46, 0);                        // yesterdayhonorpoints
        stmt.set_param(47, 0);                        // totalkills

        stmt.set_param(48, 0);                        // todaykills
        stmt.set_param(49, 0);                        // yesterdaykills
        stmt.set_param(50, 0);                        // chosentitle
        stmt.set_param(51, 0);                        // knowncurrencies
        stmt.set_param(52, 0);                        // watchedfaction
        stmt.set_param(53, 0);                        // drunk
        stmt.set_param(54, 0);                        // health
        stmt.set_param(55, 0);                        // power1
        stmt.set_param(56, 0);                        // power2
        stmt.set_param(57, 0);                        // power3

        stmt.set_param(58, 0);                        // power4
        stmt.set_param(59, 0);                        // power5
        stmt.set_param(60, 0);                        // power6
        stmt.set_param(61, 0);                        // power7
        stmt.set_param(62, 0);                        // latency
        stmt.set_param(63, 0);                        // talentgroupscount
        stmt.set_param(64, 0);                        // activetalentgroup
        stmt.set_param(65, "");                       // exploredzones
        stmt.set_param(66, "");                       // equipmentcache
        stmt.set_param(67, 0);                        // ammoid
        stmt.set_param(68, "");                       // knowntitles
        stmt.set_param(69, 0);                        // actionbars
        stmt.set_param(70, 0);                        // grantablelevels

        co_await session->server()->db()->execute_async_one<NothingRow>(stmt);
        co_return;
    } catch (const std::exception &ex) {
        Logger::get()->error("[CharHandlers::handleInsertCharacter] DB exception: {}", ex.what());
        co_return;
    }
}

boost::asio::awaitable<void> CharHandlers::handleUpdateRealmCharacters(std::shared_ptr<GameSession> session, uint8_t futureCharactersCount) {
    try {
        PreparedStatement stmt("UPDATE_REALM_CHARACTERS");
        stmt.set_param(0, futureCharactersCount);
        stmt.set_param(1, session->getAccount()->id);
        co_await session->server()->db()->execute_async_one<NothingRow>(stmt);
        co_return;
    } catch (const std::exception &ex) {
        Logger::get()->error("[CharHandlers::handleUpdateRealmCharacters] DB exception: {}", ex.what());
        co_return;
    }
}