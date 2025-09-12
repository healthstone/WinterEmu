DROP TABLE IF EXISTS dbc.dbc_questsort;

CREATE TABLE dbc.dbc_questsort
(
    id                 integer PRIMARY KEY DEFAULT 0,
    sortname_lang_enus text,
    sortname_lang_engb text,
    sortname_lang_kokr text,
    sortname_lang_frfr text,
    sortname_lang_dede text,
    sortname_lang_encn text,
    sortname_lang_zhcn text,
    sortname_lang_entw text,
    sortname_lang_zhtw text,
    sortname_lang_eses text,
    sortname_lang_esmx text,
    sortname_lang_ruru text,
    sortname_lang_ptpt text,
    sortname_lang_ptbr text,
    sortname_lang_itit text,
    sortname_lang_unk  text,
    sortname_lang_mask integer NOT NULL    DEFAULT 0
);