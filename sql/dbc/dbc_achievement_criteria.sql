DROP TABLE IF EXISTS dbc.dbc_achievement_criteria;

CREATE TABLE dbc.dbc_achievement_criteria
(
    id                    SERIAL PRIMARY KEY,
    achievement_id        INT NOT NULL DEFAULT 0,
    type                  INT NOT NULL DEFAULT 0,
    asset_id              INT NOT NULL DEFAULT 0,
    quantity              INT NOT NULL DEFAULT 0,
    start_event           INT NOT NULL DEFAULT 0,
    start_asset           INT NOT NULL DEFAULT 0,
    fail_event            INT NOT NULL DEFAULT 0,
    fail_asset            INT NOT NULL DEFAULT 0,

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
    description_lang_mask INT NOT NULL DEFAULT 0,

    flags                 INT NOT NULL DEFAULT 0,
    timer_start_event     INT NOT NULL DEFAULT 0,
    timer_asset_id        INT NOT NULL DEFAULT 0,
    timer_time            INT NOT NULL DEFAULT 0,
    ui_order              INT NOT NULL DEFAULT 0
);
