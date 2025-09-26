DROP TABLE IF EXISTS dbc.dbc_spellduration;

CREATE TABLE dbc.dbc_spellduration
(
    id                 INTEGER PRIMARY KEY DEFAULT 0,
    duration           INTEGER NOT NULL    DEFAULT 0,
    duration_per_level INTEGER NOT NULL    DEFAULT 0,
    max_duration       INTEGER NOT NULL    DEFAULT 0
);
