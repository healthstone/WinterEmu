DROP TABLE IF EXISTS dbc.dbc_transportanimation;

CREATE TABLE dbc.dbc_transportanimation
(
    id          INTEGER PRIMARY KEY DEFAULT 0,
    transportid INTEGER NOT NULL    DEFAULT 0,
    timeindex   INTEGER NOT NULL    DEFAULT 0,
    posx        REAL    NOT NULL    DEFAULT 0,
    posy        REAL    NOT NULL    DEFAULT 0,
    posz        REAL    NOT NULL    DEFAULT 0,
    sequenceid  INTEGER NOT NULL    DEFAULT 0
);
