#pragma once

#include <cstdint>

enum class CharacterLoginErrorReason : uint8_t {
    CHAR_LOGIN_NO_WORLD             = 0x01, //  Сервер не смог найти карту (map) для загрузки персонажа.
    CHAR_LOGIN_DUPLICATE_CHARACTER  = 0x02, //  Персонаж уже в игре (часто при краше или незавершённой сессии).
    CHAR_LOGIN_NO_INSTANCES         = 0x03, //  Нет доступных world-серверов / инстансов (Instance server down).
    CHAR_LOGIN_DISABLED             = 0x04, //  Этот персонаж временно отключён (GM-флаг).
    CHAR_LOGIN_NO_CHARACTER         = 0x05, //  Персонаж не найден (например, был удалён).
    CHAR_LOGIN_LOCKED_FOR_TRANSFER  = 0x06, //  Аккаунт/персонаж заблокирован (часто связано с трансфером).
    CHAR_LOGIN_LOCKED_BY_BILLING    = 0x07, //  Персонаж заблокирован из-за биллинга (например, закончилась подписка).
    CHAR_LOGIN_IN_PROGRESS          = 0x08, //  Уже выполняется процесс входа этим персонажем.
    CHAR_LOGIN_VAS_TRANSFER         = 0x09, //  (В более поздних клиентах: "персонаж в процессе трансфера"). В 3.3.5 обычно не встречается.
};