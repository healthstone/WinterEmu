#include "CharHandlers.hpp"
#include "enums/PetDefines.hpp"
#include "enums/Gender.hpp"
#include "enums/DBCStructure.h"
#include "utils/utf8utils/UTF8Utils.hpp"
#include "enums/Classes.hpp"

/** CMSG_CHAR_ENUM **/
boost::asio::awaitable<void>
CharHandlers::handleCharacterEnum(std::shared_ptr<GameSession> session) {
    Logger::get()->debug("CharHandlers::handleCharacterEnum - CMSG_CHAR_ENUM");
    auto dbcMgr = session->server()->getDBCMgr();

    WoWPacket pkt(WoWOpcodes::SMSG_CHAR_ENUM);
    // Получаем данные из базы
    auto characters = co_await fetchFromDB(session);
    if (!characters.empty()) {
        pkt.write_uint8(static_cast<uint8_t>(characters.size()));

        // Обрабатываем каждого персонажа
        for (const auto& character : characters) {
            // Пропускаем забаненных персонажей
            if (character.banned_guid.has_value()) {
                continue;
            }

            // Создаем ObjectGuid для персонажа
            ObjectGuid playerGuid = ObjectGuid::Create<HighGuid::Player>(character.guid);
            session->addLegitCharacterForAccount(playerGuid);
            if (!session->isCanCreateDK() && character.level >= 55)
                session->setIsCanCreateDK(true);

            if (static_cast<Classes>(character.class_) == Classes::CLASS_DEATH_KNIGHT) {
                session->addDKCountOnRealm();
            }

            if (session->getAccountTeam() == Team::TEAM_OTHER) {
                session->setAccountTeam(dbcMgr->teamForRace(character.race));
            }

            pkt.write_uint64_le(playerGuid.GetRawValue());

            // Основная информация о персонаже
            pkt.write_string_nt_be(character.name);    // Имя
            pkt.write_uint8(character.race);           // Раса
            pkt.write_uint8(character.class_);         // Класс
            pkt.write_uint8(character.gender);         // Пол
            pkt.write_uint8(character.skin);           // Кожа
            pkt.write_uint8(character.face);           // Лицо
            pkt.write_uint8(character.hairStyle);      // Прическа
            pkt.write_uint8(character.hairColor);      // Цвет волос
            pkt.write_uint8(character.facialStyle);    // Стиль лица
            pkt.write_uint8(character.level);          // Уровень
            pkt.write_uint32_le(character.zone);          // Зона
            pkt.write_uint32_le(character.map);           // Карта
            pkt.write_float_le(character.position_x);     // Позиция X
            pkt.write_float_le(character.position_y);     // Позиция Y
            pkt.write_float_le(character.position_z);     // Позиция Z

            if (character.guildid.has_value()) {
                pkt.write_uint32_le(character.guildid.value());
            } else {
                pkt.write_uint32_le(0);
            }

            pkt.write_uint32_le(character.playerFlags);   // Флаги игрока

            // Флаги at_login (используем только первый бит для firstLogin)
            uint8_t firstLogin = (character.at_login & 0x01) ? 1 : 0;
            pkt.write_uint8(firstLogin);

            // Данные о питомце
            pkt.write_uint32_le(character.pet_modelid.value_or(0)); // Model ID питомца
            pkt.write_uint32_le(character.pet_level.value_or(0));   // Уровень питомца
            pkt.write_uint32_le(0);                                 // Семейство питомца (не реализовано)

            // Экипировка (19 слотов)
            // TODO: Реализовать парсинг equipmentCache
            for (int i = 0; i < 19; ++i) {
                pkt.write_uint32_le(0); // Display ID
                pkt.write_uint8(0);     // Inventory type
            }

            // Дополнительные данные
            pkt.write_uint32_le(0); // Bag display ID
            pkt.write_uint8(0);     // Bag inventory type
        }
    }
    else
        pkt.write_uint8(0); // Количество персонажей (0)
    session->send_packet(std::make_shared<WoWPacket>(pkt));
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

/** CMSG_CHAR_CREATE **/
boost::asio::awaitable<void> CharHandlers::handleCharacterCreate(std::shared_ptr<GameSession> session, const std::shared_ptr<WoWPacket> &p) {
    auto log = Logger::get();
    try {
        /// User specified variables
        std::string name1    = p->read_string_nt_be();
        uint8_t race2        = p->read_uint8();
        uint8_t class3       = p->read_uint8();
        uint8_t gender4      = p->read_uint8();
        uint8_t skin5        = p->read_uint8();
        uint8_t face6        = p->read_uint8();
        uint8_t hairStyle7   = p->read_uint8();
        uint8_t hairColor8   = p->read_uint8();
        uint8_t facialHair9  = p->read_uint8();
        uint8_t outfitId10    = p->read_uint8();
        /// Server side data
        uint8_t charCount11   = 0;

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
                             "{} OutfitId\n"
                             "{} CharCount", name1, race2, class3, gender4, skin5, face6, hairStyle7, hairColor8,
                             facialHair9, outfitId10, charCount11);

        auto dbcMgr = session->server()->getDBCMgr();
        uint32_t m_expansion = session->getAccount()->expansion ? session->getAccount()->expansion.value() : 2; // default WoTLK addon = 2
        std::string m_accountName = session->getAccount()->username.value();

        ChrRacesDBC const* raceEntry = dbcMgr->getChrRacesDBC(race2);
        if (!raceEntry)
        {
            log->error("CharHandlers::handleCharacterCreate: Race ({}) not found in DBC while creating new char for account:[{}] wrong DBC files or cheater?",
                       race2, m_accountName);
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_FAILED);
            co_return;
        }

        // prevent character creating Expansion race without Expansion account
        if (raceEntry->RequiredExpansion > m_expansion)
        {
            log->error("CharHandlers::handleCharacterCreate: Expansion {} account:[{}] tried to Create character with expansion {} race ({})",
                       m_expansion, m_accountName, raceEntry->RequiredExpansion, race2);
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_EXPANSION);
            co_return;
        }

        // TC RBAC_PERM_SKIP_CHECK_CHARACTER_CREATION_RACEMASK
        if (raceEntry->Alliance == CHRRACES_ALLIANCE_TYPE_NOT_PLAYABLE || raceEntry->HasFlag(CHRRACES_FLAGS_NOT_PLAYABLE))
        {
            log->error("CharHandlers::handleCharacterCreate: Race ({}) was not playable but requested while creating new char for account:[{}] wrong DBC files or cheater?",
                       race2, m_accountName);
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_DISABLED);
            co_return;
        }

        //TC CONFIG_CHARACTER_CREATING_DISABLED_RACEMASK
        uint32_t raceMaskDisabled = 0;
        if ((1 << (race2 - 1)) & raceMaskDisabled)
        {
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_DISABLED);
            co_return;
        }

        ChrClassesDBC const* classEntry = dbcMgr->getChrClassesDBC(class3);
        if (!classEntry)
        {
            log->error("CharHandlers::handleCharacterCreate: Class ({}) not found in DBC while creating new char for account:[{}] wrong DBC files or cheater?",
                       class3, m_accountName);
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_FAILED);
            co_return;
        }

        // prevent character creating Expansion class without Expansion account
        if (classEntry->RequiredExpansion > m_expansion)
        {
            log->error("CharHandlers::handleCharacterCreate: Expansion {} account:[{}] tried to Create character with expansion {} class ({})",
                       m_expansion, m_accountName, classEntry->RequiredExpansion, class3);
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_EXPANSION_CLASS);
            co_return;
        }

        //TC CONFIG_CHARACTER_CREATING_DISABLED_CLASSMASK
        uint32_t classMaskDisabled = 0;
        if ((1 << (class3 - 1)) & classMaskDisabled)
        {
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_DISABLED);
            co_return;
        }

        if (static_cast<Classes>(class3) == Classes::CLASS_DEATH_KNIGHT)
        {
            // level check >= 55lvl exists
            if (!session->isCanCreateDK())
            {
                sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_LEVEL_REQUIREMENT);
                co_return;
            }

            // DK count check
            if (session->getDKCountOnRealm() > 0)
            {
                sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_UNIQUE_CLASS_LIMIT);
                co_return;
            }
        }

        if (session->getCharactersCountOnRealm() + 1 > MAX_CHARACTERS_PER_REALM) {
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_SERVER_LIMIT);
            co_return;
        }

        // prevent character creating with invalid name
        if (!UTF8Utils::normalizePlayerName(name1))
        {
            log->error("CharHandlers::handleCharacterCreate: Account:[{}] but tried to Create character with empty [name] ", m_accountName);
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_NAME_NO_NAME);
            co_return;
        }

        auto usernameCount = co_await fetchUsernameCountFromDB(session, name1);
        if (usernameCount > 0) {
            sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_NAME_IN_USE);
            co_return;
        }

        bool allowTwoSideAccounts = !session->server()->getRealm()->isPvPRealm();
        if (!allowTwoSideAccounts) {
            // Актуально только для последующих чаров (не первых), когда тима уже определена
            if (session->getAccountTeam() != Team::TEAM_OTHER) {
                if (session->getAccountTeam() != dbcMgr->teamForRace(race2)) {
                    sendCharResponse(session, WoWOpcodes::SMSG_CHAR_CREATE, ResponseCodes::CHAR_CREATE_PVP_TEAMS_VIOLATION);
                    co_return;
                }
            }
        }

    } catch (const std::exception &ex) {
        Logger::get()->error("[CharHandlers::handleCharacterCreate] DB exception: {}", ex.what());
        co_return;
    }
}

boost::asio::awaitable<uint64_t> CharHandlers::fetchUsernameCountFromDB(std::shared_ptr<GameSession> session, const std::string &charName) {
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

void CharHandlers::sendCharResponse(std::shared_ptr<GameSession> session, WoWOpcodes opcode, ResponseCodes result)
{
    WoWPacket pkt(opcode);
    pkt.write_uint8(static_cast<uint8_t>(result));
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}