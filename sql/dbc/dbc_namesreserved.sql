DROP TABLE IF EXISTS dbc.dbc_namesreserved;

CREATE TABLE dbc.dbc_namesreserved
(
    id       integer PRIMARY KEY DEFAULT 0,
    name     text,
    language integer NOT NULL    DEFAULT 0
);
