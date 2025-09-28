DROP TABLE IF EXISTS dbc.dbc_summonproperties;

CREATE TABLE dbc.dbc_summonproperties
(
    id      INTEGER PRIMARY KEY DEFAULT 0,
    control INTEGER NOT NULL    DEFAULT 0,
    faction INTEGER NOT NULL    DEFAULT 0,
    title   INTEGER NOT NULL    DEFAULT 0,
    slot    INTEGER NOT NULL    DEFAULT 0,
    flags   INTEGER NOT NULL    DEFAULT 0
);
