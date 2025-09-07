DROP TABLE IF EXISTS dbc.dbc_gtchancetospellcrit;

CREATE TABLE dbc.dbc_gtchancetospellcrit
(
    id   integer PRIMARY KEY DEFAULT 0,
    data float4 NOT NULL     DEFAULT 0
);
