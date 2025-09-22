DROP TABLE IF EXISTS dbc.dbc_scalingstatdistribution;

CREATE TABLE dbc.dbc_scalingstatdistribution
(
    id        INTEGER PRIMARY KEY DEFAULT 0,
    statid_1  INTEGER NOT NULL    DEFAULT 0,
    statid_2  INTEGER NOT NULL    DEFAULT 0,
    statid_3  INTEGER NOT NULL    DEFAULT 0,
    statid_4  INTEGER NOT NULL    DEFAULT 0,
    statid_5  INTEGER NOT NULL    DEFAULT 0,
    statid_6  INTEGER NOT NULL    DEFAULT 0,
    statid_7  INTEGER NOT NULL    DEFAULT 0,
    statid_8  INTEGER NOT NULL    DEFAULT 0,
    statid_9  INTEGER NOT NULL    DEFAULT 0,
    statid_10 INTEGER NOT NULL    DEFAULT 0,
    bonus_1   INTEGER NOT NULL    DEFAULT 0,
    bonus_2   INTEGER NOT NULL    DEFAULT 0,
    bonus_3   INTEGER NOT NULL    DEFAULT 0,
    bonus_4   INTEGER NOT NULL    DEFAULT 0,
    bonus_5   INTEGER NOT NULL    DEFAULT 0,
    bonus_6   INTEGER NOT NULL    DEFAULT 0,
    bonus_7   INTEGER NOT NULL    DEFAULT 0,
    bonus_8   INTEGER NOT NULL    DEFAULT 0,
    bonus_9   INTEGER NOT NULL    DEFAULT 0,
    bonus_10  INTEGER NOT NULL    DEFAULT 0,
    maxlevel  INTEGER NOT NULL    DEFAULT 0
);
