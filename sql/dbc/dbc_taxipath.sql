DROP TABLE IF EXISTS dbc.dbc_taxipath;

CREATE TABLE dbc.dbc_taxipath
(
    id             INTEGER PRIMARY KEY DEFAULT 0,
    fromtaxinode   INTEGER NOT NULL DEFAULT 0,
    totaxinode     INTEGER NOT NULL DEFAULT 0,
    cost           INTEGER NOT NULL DEFAULT 0
);
