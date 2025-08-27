#include "PlayerHandlers.hpp"
#include "src/relayserver/Entity/NodeConnector/NodeConnector.hpp"

boost::asio::awaitable<void>
PlayerHandlers::handlePlayerLogin(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    auto log = Logger::get();
    try {
        uint64_t guid = p->read_uint64_le();
        ObjectGuid playerGuid(guid);
        log->debug("PlayerHandlers::handlePlayerLogin - CMSG_PLAYER_LOGIN from {}", playerGuid.ToString());

        // Основные обязательные пакеты:
        //SMSG_CHARACTER_LOGIN_FAILED (если возникли ошибки)
        CharacterEnumRow const* character = checkCharacter(session, playerGuid);
        if (!character)
            co_return;

        // Заносим в мапу игроков playerGuid
        session->server()->addSessionInPlayerMap(playerGuid, session);

        //SMSG_LOGIN_VERIFY_WORLD - координаты и карта персонажа
        sendLoginVerifyWorld(session, character);

        //SMSG_ACCOUNT_DATA_TIMES - настройки аккаунта
        co_await session->loadAccountData(PER_CHARACTER_CACHE_MASK, playerGuid);
        session->sendAccountDataTimes(PER_CHARACTER_CACHE_MASK);

        //SMSG_TUTORIAL_FLAGS - сохраненные tutorial-флаги
        //SMSG_INITIAL_SPELLS - список заклинаний
        //SMSG_ACTION_BUTTONS - панель способностей
        //SMSG_INITIALIZE_FACTIONS - репутация с фракциями
        //SMSG_SET_PROFICIENCY - навыки владения оружием
        //SMSG_SET_REST_START - состояние отдыха
        //SMSG_BINDPOINTUPDATE - точка воскрешения
        //SMSG_LOGIN_SETTIMESPEED - игровое время
        //SMSG_UPDATE_OBJECT - данные об объектах вокруг
        //SMSG_TIME_SYNC_REQ - запрос синхронизации времени

        //Дополнительные важные пакеты:
        //SMSG_SEND_UNLEARN_SPELLS (если есть забытые заклинания)
        //SMSG_EQUIPMENT_SET_LIST (наборы экипировки)
        //SMSG_MOTD (сообщение дня)
        //SMSG_FEATURE_SYSTEM_STATUS
        //SMSG_LEARNED_DANCE_MOVES
        //SMSG_SEND_KNOWN_TITLES

        //Критически важные пакеты:
        //SMSG_UPDATE_OBJECT - создает самого персонажа и окружающие объекты
        //SMSG_LOGIN_VERIFY_WORLD - устанавливает правильное положение
        //SMSG_TIME_SYNC_REQ - синхронизирует время с клиентом


        // А теперь транслируем сам пакет
//        NodeData data;
//        data.write_uint64_le(guid);
//
//        NodePacket pkt(NodeOpcodes::REL_TO_NODE_WOWPACKET, data, *p);
//        auto nodeSession = session->server()->get_node_manager()->get_first_connector(1);
//        nodeSession->send_packet(pkt);
        co_return;

    } catch (const std::exception &ex) {
        log->error("[PlayerHandlers::handlePlayerLogin] Exception while parsing: {}", ex.what());
        co_return;
    }
}

CharacterEnumRow const* PlayerHandlers::checkCharacter(const std::shared_ptr<GameSession> &session, ObjectGuid characterGuid) {
    auto log = Logger::get();

    // Отбриваем, если игрок еще не завершил сессию
    if (session->server()->getSessionByPlayerId(characterGuid)) {
        sendCharacterError(session, CharacterLoginErrorReason::CHAR_LOGIN_DUPLICATE_CHARACTER);
        return nullptr;
    }

    // Аккаунт должен иметь в своей мапе этого чара
    auto character = session->getCharacter(characterGuid);
    if (!character) {
        Logger::get()->error("[PlayerHandlers::handlePlayerLogin]  Account ({}) can't login with that character ({})",
                             session->getAccount()->username.value(), characterGuid.ToString());
        sendCharacterError(session, CharacterLoginErrorReason::CHAR_LOGIN_NO_CHARACTER);
        return nullptr;
    }

    // Отбриваем, если персонаж в процессе трансфера
    if (character->m_isTransfer) {
        sendCharacterError(session, CharacterLoginErrorReason::CHAR_LOGIN_LOCKED_FOR_TRANSFER);
        return nullptr;
    }

    // Отбриваем, если персонаж забанен
    if (character->banned_guid) {
        sendCharacterError(session, CharacterLoginErrorReason::CHAR_LOGIN_LOCKED_BY_BILLING);
        return nullptr;
    }

    return character;
}

void PlayerHandlers::sendCharacterError(const std::shared_ptr<GameSession> &session, CharacterLoginErrorReason errorReason) {
    WoWPacket pkt(WoWOpcodes::SMSG_CHARACTER_LOGIN_FAILED);
    pkt.write_uint8(static_cast<uint8_t>(errorReason));
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}

void PlayerHandlers::sendLoginVerifyWorld(const std::shared_ptr<GameSession> &session, CharacterEnumRow const* character) {
    WoWPacket pkt(WoWOpcodes::SMSG_LOGIN_VERIFY_WORLD);
    pkt.write_int32_le(static_cast<int32_t>(character->m_map)); //int32 MapID = -1;
    pkt.write_float_le(character->m_position_x);
    pkt.write_float_le(character->m_position_y);
    pkt.write_float_le(character->m_position_z);
    pkt.write_float_le(character->m_orientation);
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}