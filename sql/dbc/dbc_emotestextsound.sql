DROP TABLE IF EXISTS dbc.dbc_emotestextsound;

CREATE TABLE dbc.dbc_emotestextsound
(
    id           integer PRIMARY KEY DEFAULT 0,
    emotestextid integer NOT NULL    DEFAULT 0,
    raceid       integer NOT NULL    DEFAULT 0,
    sexid        integer NOT NULL    DEFAULT 0,
    soundid      integer NOT NULL    DEFAULT 0
);
