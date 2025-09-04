DROP TABLE IF EXISTS dbc.dbc_dungeonencounter;

CREATE TABLE dbc.dbc_dungeonencounter
(
    id             INTEGER NOT NULL DEFAULT 0,
    mapid          INTEGER NOT NULL DEFAULT 0,
    difficulty     INTEGER NOT NULL DEFAULT 0,
    orderindex     INTEGER NOT NULL DEFAULT 0,
    bit            INTEGER NOT NULL DEFAULT 0,
    name_lang_enus TEXT,
    name_lang_engb TEXT,
    name_lang_kokr TEXT,
    name_lang_frfr TEXT,
    name_lang_dede TEXT,
    name_lang_encn TEXT,
    name_lang_zhcn TEXT,
    name_lang_entw TEXT,
    name_lang_zhtw TEXT,
    name_lang_eses TEXT,
    name_lang_esmx TEXT,
    name_lang_ruru TEXT,
    name_lang_ptpt TEXT,
    name_lang_ptbr TEXT,
    name_lang_itit TEXT,
    name_lang_unk  TEXT,
    name_lang_mask BIGINT  NOT NULL DEFAULT 0,
    spelliconid    INTEGER NOT NULL DEFAULT 0,
    PRIMARY KEY (id)
);
