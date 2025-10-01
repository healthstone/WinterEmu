DROP TABLE IF EXISTS dbc.dbc_worldmapoverlay;
-- PostgreSQL
CREATE TABLE dbc.dbc_worldmapoverlay
(
    id           INTEGER PRIMARY KEY DEFAULT 0,
    mapareaid    INTEGER NOT NULL DEFAULT 0,
    areaid_1     INTEGER NOT NULL DEFAULT 0,
    areaid_2     INTEGER NOT NULL DEFAULT 0,
    areaid_3     INTEGER NOT NULL DEFAULT 0,
    areaid_4     INTEGER NOT NULL DEFAULT 0,
    mappointx    INTEGER NOT NULL DEFAULT 0,
    mappointy    INTEGER NOT NULL DEFAULT 0,
    texturename  TEXT,
    texturewidth INTEGER NOT NULL DEFAULT 0,
    textureheight INTEGER NOT NULL DEFAULT 0,
    offsetx      INTEGER NOT NULL DEFAULT 0,
    offsety      INTEGER NOT NULL DEFAULT 0,
    hitrecttop   INTEGER NOT NULL DEFAULT 0,
    hitrectleft  INTEGER NOT NULL DEFAULT 0,
    hitrectbottom INTEGER NOT NULL DEFAULT 0,
    hitrectright INTEGER NOT NULL DEFAULT 0
);
