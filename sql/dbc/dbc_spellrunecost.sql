DROP TABLE IF EXISTS dbc.dbc_spellrunecost;

CREATE TABLE dbc.dbc_spellrunecost
(
    id         INTEGER PRIMARY KEY DEFAULT 0,
    blood      INTEGER NOT NULL    DEFAULT 0,
    unholy     INTEGER NOT NULL    DEFAULT 0,
    frost      INTEGER NOT NULL    DEFAULT 0,
    runicpower INTEGER NOT NULL    DEFAULT 0
);
