DROP TABLE IF EXISTS dbc.dbc_areatable;

CREATE TABLE dbc.dbc_areatable
(
    id                             SERIAL PRIMARY KEY, -- AUTO_INCREMENT в MySQL заменяем на SERIAL/PRIMARY KEY
    continent_id                   INT  NOT NULL DEFAULT 0,
    parent_area_id                 INT  NOT NULL DEFAULT 0,
    area_bit                       INT  NOT NULL DEFAULT 0,
    flags                          INT  NOT NULL DEFAULT 0,
    sound_provider_pref            INT  NOT NULL DEFAULT 0,
    sound_provider_pref_underwater INT  NOT NULL DEFAULT 0,
    ambience_id                    INT  NOT NULL DEFAULT 0,
    zone_music                     INT  NOT NULL DEFAULT 0,
    intro_sound                    INT  NOT NULL DEFAULT 0,
    exploration_level              INT  NOT NULL DEFAULT 0,

    area_name_lang_enUS            TEXT,
    area_name_lang_enGB            TEXT,
    area_name_lang_koKR            TEXT,
    area_name_lang_frFR            TEXT,
    area_name_lang_deDE            TEXT,
    area_name_lang_enCN            TEXT,
    area_name_lang_zhCN            TEXT,
    area_name_lang_enTW            TEXT,
    area_name_lang_zhTW            TEXT,
    area_name_lang_esES            TEXT,
    area_name_lang_esMX            TEXT,
    area_name_lang_ruRU            TEXT,
    area_name_lang_ptPT            TEXT,
    area_name_lang_ptBR            TEXT,
    area_name_lang_itIT            TEXT,
    area_name_lang_Unk             TEXT,

    area_name_lang_mask            INT  NOT NULL DEFAULT 0,
    faction_group_mask             INT  NOT NULL DEFAULT 0,
    liquid_type_id_1               INT  NOT NULL DEFAULT 0,
    liquid_type_id_2               INT  NOT NULL DEFAULT 0,
    liquid_type_id_3               INT  NOT NULL DEFAULT 0,
    liquid_type_id_4               INT  NOT NULL DEFAULT 0,
    min_elevation                  REAL NOT NULL DEFAULT 0,
    ambient_multiplier             REAL NOT NULL DEFAULT 0,
    lightid                        INT  NOT NULL DEFAULT 0
);
