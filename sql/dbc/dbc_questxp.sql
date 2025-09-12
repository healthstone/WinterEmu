DROP TABLE IF EXISTS dbc.dbc_questxp;

CREATE TABLE dbc.dbc_questxp
(
    id            integer PRIMARY KEY DEFAULT 0,
    difficulty_1  integer NOT NULL    DEFAULT 0,
    difficulty_2  integer NOT NULL    DEFAULT 0,
    difficulty_3  integer NOT NULL    DEFAULT 0,
    difficulty_4  integer NOT NULL    DEFAULT 0,
    difficulty_5  integer NOT NULL    DEFAULT 0,
    difficulty_6  integer NOT NULL    DEFAULT 0,
    difficulty_7  integer NOT NULL    DEFAULT 0,
    difficulty_8  integer NOT NULL    DEFAULT 0,
    difficulty_9  integer NOT NULL    DEFAULT 0,
    difficulty_10 integer NOT NULL    DEFAULT 0
);
