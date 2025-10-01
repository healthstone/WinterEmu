DROP TABLE IF EXISTS dbc.dbc_worldsafelocs;

CREATE TABLE dbc.dbc_worldsafelocs
(
    id                  INTEGER PRIMARY KEY DEFAULT 0,
    continent           INTEGER NOT NULL    DEFAULT 0,
    loc_x               REAL    NOT NULL    DEFAULT 0,
    loc_y               REAL    NOT NULL    DEFAULT 0,
    loc_z               REAL    NOT NULL    DEFAULT 0,
    area_name_lang_enus TEXT,
    area_name_lang_engb TEXT,
    area_name_lang_kokr TEXT,
    area_name_lang_frfr TEXT,
    area_name_lang_dede TEXT,
    area_name_lang_encn TEXT,
    area_name_lang_zhcn TEXT,
    area_name_lang_entw TEXT,
    area_name_lang_zhtw TEXT,
    area_name_lang_eses TEXT,
    area_name_lang_esmx TEXT,
    area_name_lang_ruru TEXT,
    area_name_lang_ptpt TEXT,
    area_name_lang_ptbr TEXT,
    area_name_lang_itit TEXT,
    area_name_lang_unk  TEXT,
    area_name_lang_mask BIGINT  NOT NULL    DEFAULT 0
);
