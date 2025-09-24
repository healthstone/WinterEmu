DROP TABLE IF EXISTS dbc.dbc_skilllineability;

CREATE TABLE dbc.dbc_skilllineability
(
    id                       INTEGER PRIMARY KEY DEFAULT 0,
    skillline                INTEGER NOT NULL    DEFAULT 0,
    spell                    INTEGER NOT NULL    DEFAULT 0,
    racemask                 INTEGER NOT NULL    DEFAULT 0,
    classmask                INTEGER NOT NULL    DEFAULT 0,
    excluderace              INTEGER NOT NULL    DEFAULT 0,
    excludeclass             INTEGER NOT NULL    DEFAULT 0,
    minskilllinerank         INTEGER NOT NULL    DEFAULT 0,
    supercededbyspell        INTEGER NOT NULL    DEFAULT 0,
    acquiremethod            INTEGER NOT NULL    DEFAULT 0,
    trivialskilllinerankhigh INTEGER NOT NULL    DEFAULT 0,
    trivialskilllineranklow  INTEGER NOT NULL    DEFAULT 0,
    tradeskillcategoryid     INTEGER NOT NULL    DEFAULT 0
);
