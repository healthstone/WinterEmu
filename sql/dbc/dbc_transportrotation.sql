DROP TABLE IF EXISTS dbc.dbc_transportrotation;

CREATE TABLE dbc.dbc_transportrotation
(
    id            INTEGER PRIMARY KEY DEFAULT 0,
    gameobjectsid INTEGER NOT NULL    DEFAULT 0,
    timeindex     INTEGER NOT NULL    DEFAULT 0,
    rotx          REAL    NOT NULL    DEFAULT 0,
    roty          REAL    NOT NULL    DEFAULT 0,
    rotz          REAL    NOT NULL    DEFAULT 0,
    rotw          REAL    NOT NULL    DEFAULT 0
);
