DROP TABLE IF EXISTS dbc.dbc_talent;

CREATE TABLE dbc.dbc_talent
(
    id              INTEGER PRIMARY KEY DEFAULT 0,
    tabid           INTEGER NOT NULL    DEFAULT 0,
    tierid          INTEGER NOT NULL    DEFAULT 0,
    columnindex     INTEGER NOT NULL    DEFAULT 0,
    spellrank_1     INTEGER NOT NULL    DEFAULT 0,
    spellrank_2     INTEGER NOT NULL    DEFAULT 0,
    spellrank_3     INTEGER NOT NULL    DEFAULT 0,
    spellrank_4     INTEGER NOT NULL    DEFAULT 0,
    spellrank_5     INTEGER NOT NULL    DEFAULT 0,
    spellrank_6     INTEGER NOT NULL    DEFAULT 0,
    spellrank_7     INTEGER NOT NULL    DEFAULT 0,
    spellrank_8     INTEGER NOT NULL    DEFAULT 0,
    spellrank_9     INTEGER NOT NULL    DEFAULT 0,
    prereqtalent_1  INTEGER NOT NULL    DEFAULT 0,
    prereqtalent_2  INTEGER NOT NULL    DEFAULT 0,
    prereqtalent_3  INTEGER NOT NULL    DEFAULT 0,
    prerekrank_1    INTEGER NOT NULL    DEFAULT 0,
    prerekrank_2    INTEGER NOT NULL    DEFAULT 0,
    prerekrank_3    INTEGER NOT NULL    DEFAULT 0,
    flags           INTEGER NOT NULL    DEFAULT 0,
    requiredspellid INTEGER NOT NULL    DEFAULT 0,
    categorymask_1  INTEGER NOT NULL    DEFAULT 0,
    categorymask_2  INTEGER NOT NULL    DEFAULT 0
);
