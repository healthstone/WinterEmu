DROP TABLE IF EXISTS dbc.dbc_auctionhouse;

CREATE TABLE dbc.dbc_auctionhouse
(
    id               SERIAL PRIMARY KEY,
    faction_id       INTEGER NOT NULL DEFAULT 0,
    deposit_rate     INTEGER NOT NULL DEFAULT 0,
    consignment_rate INTEGER NOT NULL DEFAULT 0,

    name_lang_enUS   TEXT,
    name_lang_enGB   TEXT,
    name_lang_koKR   TEXT,
    name_lang_frFR   TEXT,
    name_lang_deDE   TEXT,
    name_lang_enCN   TEXT,
    name_lang_zhCN   TEXT,
    name_lang_enTW   TEXT,
    name_lang_zhTW   TEXT,
    name_lang_esES   TEXT,
    name_lang_esMX   TEXT,
    name_lang_ruRU   TEXT,
    name_lang_ptPT   TEXT,
    name_lang_ptBR   TEXT,
    name_lang_itIT   TEXT,
    name_lang_unk    TEXT,

    name_lang_mask   INTEGER NOT NULL DEFAULT 0
);
