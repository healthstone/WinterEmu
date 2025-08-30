DROP TABLE IF EXISTS dbc.dbc_barbershopstyle;

CREATE TABLE dbc.dbc_barbershopstyle
(
    id                    INTEGER NOT NULL DEFAULT 0 PRIMARY KEY,
    type                  INTEGER NOT NULL DEFAULT 0,

    displayname_lang_enus TEXT,
    displayname_lang_engb TEXT,
    displayname_lang_kokr TEXT,
    displayname_lang_frfr TEXT,
    displayname_lang_dede TEXT,
    displayname_lang_encn TEXT,
    displayname_lang_zhcn TEXT,
    displayname_lang_entw TEXT,
    displayname_lang_zhtw TEXT,
    displayname_lang_eses TEXT,
    displayname_lang_esmx TEXT,
    displayname_lang_ruru TEXT,
    displayname_lang_ptpt TEXT,
    displayname_lang_ptbr TEXT,
    displayname_lang_itit TEXT,
    displayname_lang_unk  TEXT,
    displayname_lang_mask BIGINT  NOT NULL DEFAULT 0,

    description_lang_enus TEXT,
    description_lang_engb TEXT,
    description_lang_kokr TEXT,
    description_lang_frfr TEXT,
    description_lang_dede TEXT,
    description_lang_encn TEXT,
    description_lang_zhcn TEXT,
    description_lang_entw TEXT,
    description_lang_zhtw TEXT,
    description_lang_eses TEXT,
    description_lang_esmx TEXT,
    description_lang_ruru TEXT,
    description_lang_ptpt TEXT,
    description_lang_ptbr TEXT,
    description_lang_itit TEXT,
    description_lang_unk  TEXT,
    description_lang_mask BIGINT  NOT NULL DEFAULT 0,

    cost_modifier         REAL    NOT NULL DEFAULT 0,
    race                  INTEGER NOT NULL DEFAULT 0,
    sex                   INTEGER NOT NULL DEFAULT 0,
    data                  INTEGER NOT NULL DEFAULT 0
);
