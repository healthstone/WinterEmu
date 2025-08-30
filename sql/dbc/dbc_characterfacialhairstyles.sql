DROP TABLE IF EXISTS dbc.dbc_characterfacialhairstyles;

CREATE TABLE dbc.dbc_characterfacialhairstyles
(
    id          INTEGER NOT NULL DEFAULT 0,
    raceid      INTEGER NOT NULL DEFAULT 0,
    sexid       INTEGER NOT NULL DEFAULT 0,
    variationid INTEGER NOT NULL DEFAULT 0,
    geoset_1    INTEGER NOT NULL DEFAULT 0,
    geoset_2    INTEGER NOT NULL DEFAULT 0,
    geoset_3    INTEGER NOT NULL DEFAULT 0,
    geoset_4    INTEGER NOT NULL DEFAULT 0,
    geoset_5    INTEGER NOT NULL DEFAULT 0,
    CONSTRAINT pk_dbc_characterfacialhairstyles PRIMARY KEY (id)
);
