DROP TABLE IF EXISTS dbc.dbc_worldmaparea;

CREATE TABLE dbc.dbc_worldmaparea
(
    id                  INTEGER PRIMARY KEY DEFAULT 0,
    mapid               INTEGER NOT NULL    DEFAULT 0,
    areaid              INTEGER NOT NULL    DEFAULT 0,
    areaname            TEXT,
    locleft             REAL    NOT NULL    DEFAULT 0,
    locright            REAL    NOT NULL    DEFAULT 0,
    loctop              REAL    NOT NULL    DEFAULT 0,
    locbottom           REAL    NOT NULL    DEFAULT 0,
    displaymapid        INTEGER NOT NULL    DEFAULT 0,
    defaultdungeonfloor INTEGER NOT NULL    DEFAULT 0,
    parentworldmapid    INTEGER NOT NULL    DEFAULT 0
);
