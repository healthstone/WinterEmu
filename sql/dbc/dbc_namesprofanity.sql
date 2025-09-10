DROP TABLE IF EXISTS dbc.dbc_namesprofanity;

CREATE TABLE dbc.dbc_namesprofanity
(
    id       integer PRIMARY KEY DEFAULT 0,
    name     text,
    language integer NOT NULL    DEFAULT 0
);
