DROP TABLE IF EXISTS dbc.dbc_emotestext;

CREATE TABLE dbc.dbc_emotestext
(
    id           integer PRIMARY KEY DEFAULT 0,
    name         text,
    emoteid      integer NOT NULL    DEFAULT 0,
    emotetext_1  integer NOT NULL    DEFAULT 0,
    emotetext_2  integer NOT NULL    DEFAULT 0,
    emotetext_3  integer NOT NULL    DEFAULT 0,
    emotetext_4  integer NOT NULL    DEFAULT 0,
    emotetext_5  integer NOT NULL    DEFAULT 0,
    emotetext_6  integer NOT NULL    DEFAULT 0,
    emotetext_7  integer NOT NULL    DEFAULT 0,
    emotetext_8  integer NOT NULL    DEFAULT 0,
    emotetext_9  integer NOT NULL    DEFAULT 0,
    emotetext_10 integer NOT NULL    DEFAULT 0,
    emotetext_11 integer NOT NULL    DEFAULT 0,
    emotetext_12 integer NOT NULL    DEFAULT 0,
    emotetext_13 integer NOT NULL    DEFAULT 0,
    emotetext_14 integer NOT NULL    DEFAULT 0,
    emotetext_15 integer NOT NULL    DEFAULT 0,
    emotetext_16 integer NOT NULL    DEFAULT 0
);
