DROP TABLE IF EXISTS dbc.dbc_areagroup;

CREATE TABLE dbc.dbc_areagroup
(
    id         INTEGER NOT NULL DEFAULT 0,
    areaid_1   INTEGER NOT NULL DEFAULT 0,
    areaid_2   INTEGER NOT NULL DEFAULT 0,
    areaid_3   INTEGER NOT NULL DEFAULT 0,
    areaid_4   INTEGER NOT NULL DEFAULT 0,
    areaid_5   INTEGER NOT NULL DEFAULT 0,
    areaid_6   INTEGER NOT NULL DEFAULT 0,
    nextareaid INTEGER NOT NULL DEFAULT 0,
    PRIMARY KEY (id)
);
