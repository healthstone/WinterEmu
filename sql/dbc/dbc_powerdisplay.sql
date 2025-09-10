DROP TABLE IF EXISTS dbc.dbc_powerdisplay;

CREATE TABLE dbc.dbc_powerdisplay
(
    id                   integer PRIMARY KEY DEFAULT 0,
    actualtype           integer  NOT NULL   DEFAULT 0,
    globalstring_basetag text,
    red                  smallint NOT NULL   DEFAULT 0,
    green                smallint NOT NULL   DEFAULT 0,
    blue                 smallint NOT NULL   DEFAULT 0
);

INSERT INTO dbc.dbc_powerdisplay (id, actualtype, globalstring_basetag, red, green, blue)
VALUES (1, 0, 'AMMOSLOT', 255, 128, 0),
       (41, 3, 'POWER_TYPE_PYRITE', 0, 202, 255),
       (61, 3, 'POWER_TYPE_STEAM', 242, 242, 242),
       (101, 3, 'POWER_TYPE_HEAT', 255, 125, 0),
       (121, 3, 'POWER_TYPE_OOZE', 193, 255, 0),
       (141, 3, 'POWER_TYPE_BLOOD_POWER', 188, 0, 255),
       (142, 3, 'POWER_TYPE_WRATH', 255, 176, 0);
