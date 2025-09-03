DROP TABLE IF EXISTS dbc.dbc_creaturefamily;

CREATE TABLE dbc.dbc_creaturefamily
(
    id             integer NOT NULL DEFAULT 0,
    minscale       float4  NOT NULL DEFAULT 0,
    minscalelevel  integer NOT NULL DEFAULT 0,
    maxscale       float4  NOT NULL DEFAULT 0,
    maxscalelevel  integer NOT NULL DEFAULT 0,
    skillline_1    integer NOT NULL DEFAULT 0,
    skillline_2    integer NOT NULL DEFAULT 0,
    petfoodmask    integer NOT NULL DEFAULT 0,
    pettalenttype  integer NOT NULL DEFAULT 0,
    categoryenumid integer NOT NULL DEFAULT 0,

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

    name_lang_mask bigint  NOT NULL DEFAULT 0,
    iconfile       text,

    CONSTRAINT pk_dbc_creaturefamily PRIMARY KEY (id)
);
