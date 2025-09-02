DROP TABLE IF EXISTS dbc.dbc_creaturedisplayinfo;

CREATE TABLE dbc.dbc_creaturedisplayinfo
(
    id                    integer PRIMARY KEY DEFAULT 0,
    modelid               integer NOT NULL    DEFAULT 0,
    soundid               integer NOT NULL    DEFAULT 0,
    extendeddisplayinfoid integer NOT NULL    DEFAULT 0,
    creaturemodelscale    float4  NOT NULL    DEFAULT 0,
    creaturemodelalpha    integer NOT NULL    DEFAULT 0,
    texturevariation_1    text,
    texturevariation_2    text,
    texturevariation_3    text,
    portraittexturename   text,
    bloodlevel            integer NOT NULL    DEFAULT 0,
    bloodid               integer NOT NULL    DEFAULT 0,
    npcsoundid            integer NOT NULL    DEFAULT 0,
    particlecolorid       integer NOT NULL    DEFAULT 0,
    creaturegeosetdata    integer NOT NULL    DEFAULT 0,
    objecteffectpackageid integer NOT NULL    DEFAULT 0
);
