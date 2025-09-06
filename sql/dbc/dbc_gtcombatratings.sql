DROP TABLE IF EXISTS dbc.dbc_gtcombatratings;

CREATE TABLE dbc.dbc_gtcombatratings
(
    id   integer PRIMARY KEY DEFAULT 0,
    data float4 NOT NULL     DEFAULT 0
);
