DROP TABLE IF EXISTS dbc.dbc_wmoareatable;

CREATE TABLE dbc.dbc_wmoareatable
(
    id                          INTEGER PRIMARY KEY DEFAULT 0,
    wmoid                       INTEGER NOT NULL    DEFAULT 0,
    namesetid                   INTEGER NOT NULL    DEFAULT 0,
    wmogroupid                  INTEGER NOT NULL    DEFAULT 0,
    soundproviderpref           INTEGER NOT NULL    DEFAULT 0,
    soundproviderprefunderwater INTEGER NOT NULL    DEFAULT 0,
    ambienceid                  INTEGER NOT NULL    DEFAULT 0,
    zonemusic                   INTEGER NOT NULL    DEFAULT 0,
    introsound                  INTEGER NOT NULL    DEFAULT 0,
    flags                       INTEGER NOT NULL    DEFAULT 0,
    areatableid                 INTEGER NOT NULL    DEFAULT 0,
    areaname_lang_enus          TEXT,
    areaname_lang_engb          TEXT,
    areaname_lang_kokr          TEXT,
    areaname_lang_frfr          TEXT,
    areaname_lang_dede          TEXT,
    areaname_lang_encn          TEXT,
    areaname_lang_zhcn          TEXT,
    areaname_lang_entw          TEXT,
    areaname_lang_zhtw          TEXT,
    areaname_lang_eses          TEXT,
    areaname_lang_esmx          TEXT,
    areaname_lang_ruru          TEXT,
    areaname_lang_ptpt          TEXT,
    areaname_lang_ptbr          TEXT,
    areaname_lang_itit          TEXT,
    areaname_lang_unk           TEXT,
    areaname_lang_mask          BIGINT  NOT NULL    DEFAULT 0
);
