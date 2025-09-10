DROP TABLE IF EXISTS dbc.dbc_overridespelldata;

CREATE TABLE dbc.dbc_overridespelldata
(
    id        integer PRIMARY KEY DEFAULT 0,

    spells_1  integer NOT NULL    DEFAULT 0,
    spells_2  integer NOT NULL    DEFAULT 0,
    spells_3  integer NOT NULL    DEFAULT 0,
    spells_4  integer NOT NULL    DEFAULT 0,
    spells_5  integer NOT NULL    DEFAULT 0,
    spells_6  integer NOT NULL    DEFAULT 0,
    spells_7  integer NOT NULL    DEFAULT 0,
    spells_8  integer NOT NULL    DEFAULT 0,
    spells_9  integer NOT NULL    DEFAULT 0,
    spells_10 integer NOT NULL    DEFAULT 0,

    flags     integer NOT NULL    DEFAULT 0
);

INSERT INTO dbc.dbc_overridespelldata
(id, spells_1, spells_2, spells_3, spells_4, spells_5, spells_6, spells_7, spells_8, spells_9, spells_10, flags)
VALUES
    (1,   40430, 59351, 31262, 61721, 0,     0,     0,     0,     0,     0, 5),
    (61,  62634, 62479, 62473, 62471, 0, 62428,     0,     0,     0,     0, 0),
    (121, 66905,     0,     0,     0, 0,     0,     0,     0,     0,     0, 1),
    (141,     0,     0,     0,     0, 0,     0,     0,     0,     0,     0, 0),
    (221, 69667, 69668, 69671, 69669, 0,     0,     0,     0,     0,     0, 0),
    (241, 70946,     0,     0,     0, 0,     0,     0,     0,     0,     0, 0),
    (266, 73491, 73492, 73499,     0, 0,     0,     0,     0,     0,     0, 0),
    (271, 33912, 31289, 24331,     0, 0,     0,     0,     0,     0,     0, 0);

