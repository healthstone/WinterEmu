DROP TABLE IF EXISTS dbc.dbc_charsections;

CREATE TABLE dbc.dbc_charsections
(
    id             INTEGER NOT NULL DEFAULT 0,
    raceid         INTEGER NOT NULL DEFAULT 0,
    sexid          INTEGER NOT NULL DEFAULT 0,
    basesection    INTEGER NOT NULL DEFAULT 0,
    texturename_1  TEXT,
    texturename_2  TEXT,
    texturename_3  TEXT,
    flags          INTEGER NOT NULL DEFAULT 0,
    variationindex INTEGER NOT NULL DEFAULT 0,
    colorindex     INTEGER NOT NULL DEFAULT 0,
    CONSTRAINT pk_dbc_charsections PRIMARY KEY (id)
);
