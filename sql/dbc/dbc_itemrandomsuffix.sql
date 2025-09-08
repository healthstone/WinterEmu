DROP TABLE IF EXISTS dbc.dbc_itemrandomsuffix;

CREATE TABLE dbc.dbc_itemrandomsuffix
(
    id              integer PRIMARY KEY DEFAULT 0,
    name_lang_enus  text,
    name_lang_engb  text,
    name_lang_kokr  text,
    name_lang_frfr  text,
    name_lang_dede  text,
    name_lang_encn  text,
    name_lang_zhcn  text,
    name_lang_entw  text,
    name_lang_zhtw  text,
    name_lang_eses  text,
    name_lang_esmx  text,
    name_lang_ruru  text,
    name_lang_ptpt  text,
    name_lang_ptbr  text,
    name_lang_itit  text,
    name_lang_unk   text,
    name_lang_mask  bigint  NOT NULL    DEFAULT 0,
    internalname    text,
    enchantment_1   integer NOT NULL    DEFAULT 0,
    enchantment_2   integer NOT NULL    DEFAULT 0,
    enchantment_3   integer NOT NULL    DEFAULT 0,
    enchantment_4   integer NOT NULL    DEFAULT 0,
    enchantment_5   integer NOT NULL    DEFAULT 0,
    allocationpct_1 integer NOT NULL    DEFAULT 0,
    allocationpct_2 integer NOT NULL    DEFAULT 0,
    allocationpct_3 integer NOT NULL    DEFAULT 0,
    allocationpct_4 integer NOT NULL    DEFAULT 0,
    allocationpct_5 integer NOT NULL    DEFAULT 0
);
