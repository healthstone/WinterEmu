#include "CharHandlers.hpp"
#include "enums/PetDefines.hpp"

/** CMSG_CHAR_ENUM **/
boost::asio::awaitable<void>
CharHandlers::handleCharacterEnum(const std::shared_ptr<GameSession> &session) {
    Logger::get()->debug("CharHandlers::handleCharacterEnum - CMSG_CHAR_ENUM");

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
        stmt.set_param(1, session->getAccountId());
        auto rows = co_await session->server()->db()->execute_async_many<CharacterEnumRow>(stmt);
        co_return rows;
    } catch (const std::exception &ex) {
        Logger::get()->error("[fetchFromDB] DB exception: {}", ex.what());
        co_return std::vector<CharacterEnumRow>{};
    }
}