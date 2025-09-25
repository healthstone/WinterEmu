DROP TABLE IF EXISTS dbc.dbc_spellcasttimes;

CREATE TABLE dbc.dbc_spellcasttimes
(
    id        INTEGER PRIMARY KEY DEFAULT 0,
    base      INTEGER NOT NULL DEFAULT 0,
    perlevel  INTEGER NOT NULL DEFAULT 0,
    minimum   INTEGER NOT NULL DEFAULT 0
);
