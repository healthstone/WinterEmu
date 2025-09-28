DROP TABLE IF EXISTS dbc.dbc_teamcontributionpoints;

CREATE TABLE dbc.dbc_teamcontributionpoints
(
    id   INTEGER PRIMARY KEY DEFAULT 0,
    data REAL NOT NULL       DEFAULT 0
);
