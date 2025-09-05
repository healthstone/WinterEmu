DROP TABLE IF EXISTS dbc.dbc_emotes;

CREATE TABLE dbc.dbc_emotes
(
    id                 integer PRIMARY KEY DEFAULT 0,
    emoteslashcommand  text,
    animid             integer NOT NULL    DEFAULT 0,
    emoteflags         integer NOT NULL    DEFAULT 0,
    emotespecproc      integer NOT NULL    DEFAULT 0,
    emotespecprocparam integer NOT NULL    DEFAULT 0,
    eventsoundid       integer NOT NULL    DEFAULT 0
);
