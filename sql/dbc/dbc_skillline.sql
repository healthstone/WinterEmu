DROP TABLE IF EXISTS dbc.dbc_skillline;

CREATE TABLE dbc.dbc_skillline
(
    ID                      integer NOT NULL DEFAULT 0,
    CategoryID              integer NOT NULL DEFAULT 0,
    SkillCostsID            integer NOT NULL DEFAULT 0,

    -- DisplayName
    DisplayName_Lang_enUS   TEXT,
    DisplayName_Lang_enGB   TEXT,
    DisplayName_Lang_koKR   TEXT,
    DisplayName_Lang_frFR   TEXT,
    DisplayName_Lang_deDE   TEXT,
    DisplayName_Lang_enCN   TEXT,
    DisplayName_Lang_zhCN   TEXT,
    DisplayName_Lang_enTW   TEXT,
    DisplayName_Lang_zhTW   TEXT,
    DisplayName_Lang_esES   TEXT,
    DisplayName_Lang_esMX   TEXT,
    DisplayName_Lang_ruRU   TEXT,
    DisplayName_Lang_ptPT   TEXT,
    DisplayName_Lang_ptBR   TEXT,
    DisplayName_Lang_itIT   TEXT,
    DisplayName_Lang_Unk    TEXT,
    DisplayName_Lang_Mask   bigint  NOT NULL DEFAULT 0,

    -- Description
    Description_Lang_enUS   TEXT,
    Description_Lang_enGB   TEXT,
    Description_Lang_koKR   TEXT,
    Description_Lang_frFR   TEXT,
    Description_Lang_deDE   TEXT,
    Description_Lang_enCN   TEXT,
    Description_Lang_zhCN   TEXT,
    Description_Lang_enTW   TEXT,
    Description_Lang_zhTW   TEXT,
    Description_Lang_esES   TEXT,
    Description_Lang_esMX   TEXT,
    Description_Lang_ruRU   TEXT,
    Description_Lang_ptPT   TEXT,
    Description_Lang_ptBR   TEXT,
    Description_Lang_itIT   TEXT,
    Description_Lang_Unk    TEXT,
    Description_Lang_Mask   bigint  NOT NULL DEFAULT 0,

    SpellIconID             integer NOT NULL DEFAULT 0,

    -- AlternateVerb
    AlternateVerb_Lang_enUS TEXT,
    AlternateVerb_Lang_enGB TEXT,
    AlternateVerb_Lang_koKR TEXT,
    AlternateVerb_Lang_frFR TEXT,
    AlternateVerb_Lang_deDE TEXT,
    AlternateVerb_Lang_enCN TEXT,
    AlternateVerb_Lang_zhCN TEXT,
    AlternateVerb_Lang_enTW TEXT,
    AlternateVerb_Lang_zhTW TEXT,
    AlternateVerb_Lang_esES TEXT,
    AlternateVerb_Lang_esMX TEXT,
    AlternateVerb_Lang_ruRU TEXT,
    AlternateVerb_Lang_ptPT TEXT,
    AlternateVerb_Lang_ptBR TEXT,
    AlternateVerb_Lang_itIT TEXT,
    AlternateVerb_Lang_Unk  TEXT,
    AlternateVerb_Lang_Mask bigint  NOT NULL DEFAULT 0,

    CanLink                 integer NOT NULL DEFAULT 0,

    PRIMARY KEY (ID)
);
