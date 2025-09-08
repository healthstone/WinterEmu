DROP TABLE IF EXISTS dbc.dbc_itemlimitcategory;

CREATE TABLE dbc.dbc_itemlimitcategory
(
    id             integer PRIMARY KEY DEFAULT 0,
    name_lang_enus text,
    name_lang_engb text,
    name_lang_kokr text,
    name_lang_frfr text,
    name_lang_dede text,
    name_lang_encn text,
    name_lang_zhcn text,
    name_lang_entw text,
    name_lang_zhtw text,
    name_lang_eses text,
    name_lang_esmx text,
    name_lang_ruru text,
    name_lang_ptpt text,
    name_lang_ptbr text,
    name_lang_itit text,
    name_lang_unk  text,
    name_lang_mask integer NOT NULL    DEFAULT 0,
    quantity       integer NOT NULL    DEFAULT 0,
    flags          integer NOT NULL    DEFAULT 0
);
