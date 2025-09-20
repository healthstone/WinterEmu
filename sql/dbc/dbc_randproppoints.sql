DROP TABLE IF EXISTS dbc.dbc_randproppoints;

CREATE TABLE dbc.dbc_randproppoints
(
    id         integer PRIMARY KEY DEFAULT 0,
    epic_1     integer NOT NULL    DEFAULT 0,
    epic_2     integer NOT NULL    DEFAULT 0,
    epic_3     integer NOT NULL    DEFAULT 0,
    epic_4     integer NOT NULL    DEFAULT 0,
    epic_5     integer NOT NULL    DEFAULT 0,
    superior_1 integer NOT NULL    DEFAULT 0,
    superior_2 integer NOT NULL    DEFAULT 0,
    superior_3 integer NOT NULL    DEFAULT 0,
    superior_4 integer NOT NULL    DEFAULT 0,
    superior_5 integer NOT NULL    DEFAULT 0,
    good_1     integer NOT NULL    DEFAULT 0,
    good_2     integer NOT NULL    DEFAULT 0,
    good_3     integer NOT NULL    DEFAULT 0,
    good_4     integer NOT NULL    DEFAULT 0,
    good_5     integer NOT NULL    DEFAULT 0
);
