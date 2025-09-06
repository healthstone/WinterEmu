DROP TABLE IF EXISTS dbc.dbc_gameobjectdisplayinfo;

CREATE TABLE dbc.dbc_gameobjectdisplayinfo
(
    id                    integer PRIMARY KEY DEFAULT 0,
    modelname             text,
    sound_1               integer NOT NULL    DEFAULT 0,
    sound_2               integer NOT NULL    DEFAULT 0,
    sound_3               integer NOT NULL    DEFAULT 0,
    sound_4               integer NOT NULL    DEFAULT 0,
    sound_5               integer NOT NULL    DEFAULT 0,
    sound_6               integer NOT NULL    DEFAULT 0,
    sound_7               integer NOT NULL    DEFAULT 0,
    sound_8               integer NOT NULL    DEFAULT 0,
    sound_9               integer NOT NULL    DEFAULT 0,
    sound_10              integer NOT NULL    DEFAULT 0,
    geoboxminx            float4  NOT NULL    DEFAULT 0,
    geoboxminy            float4  NOT NULL    DEFAULT 0,
    geoboxminz            float4  NOT NULL    DEFAULT 0,
    geoboxmaxx            float4  NOT NULL    DEFAULT 0,
    geoboxmaxy            float4  NOT NULL    DEFAULT 0,
    geoboxmaxz            float4  NOT NULL    DEFAULT 0,
    objecteffectpackageid integer NOT NULL    DEFAULT 0
);
