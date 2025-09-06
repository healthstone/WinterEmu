DROP TABLE IF EXISTS dbc.dbc_gemproperties;

CREATE TABLE dbc.dbc_gemproperties
(
    id            integer PRIMARY KEY DEFAULT 0,
    enchant_id    integer NOT NULL    DEFAULT 0,
    maxcount_inv  integer NOT NULL    DEFAULT 0,
    maxcount_item integer NOT NULL    DEFAULT 0,
    type          integer NOT NULL    DEFAULT 0
);
