DROP TABLE IF EXISTS dbc.dbc_creaturemodeldata;

CREATE TABLE dbc.dbc_creaturemodeldata
(
    id                     integer NOT NULL DEFAULT 0,
    flags                  integer NOT NULL DEFAULT 0,
    modelname              text,
    sizeclass              integer NOT NULL DEFAULT 0,
    modelscale             float4  NOT NULL DEFAULT 0,
    bloodid                integer NOT NULL DEFAULT 0,
    footprinttextureid     integer NOT NULL DEFAULT 0,
    footprinttexturelength float4  NOT NULL DEFAULT 0,
    footprinttexturewidth  float4  NOT NULL DEFAULT 0,
    footprintparticlescale float4  NOT NULL DEFAULT 0,
    foleymaterialid        integer NOT NULL DEFAULT 0,
    footstepshakesize      integer NOT NULL DEFAULT 0,
    deaththudshakesize     integer NOT NULL DEFAULT 0,
    soundid                integer NOT NULL DEFAULT 0,
    collisionwidth         float4  NOT NULL DEFAULT 0,
    collisionheight        float4  NOT NULL DEFAULT 0,
    mountheight            float4  NOT NULL DEFAULT 0,
    geoboxminx             float4  NOT NULL DEFAULT 0,
    geoboxminy             float4  NOT NULL DEFAULT 0,
    geoboxminz             float4  NOT NULL DEFAULT 0,
    geoboxmaxx             float4  NOT NULL DEFAULT 0,
    geoboxmaxy             float4  NOT NULL DEFAULT 0,
    geoboxmaxz             float4  NOT NULL DEFAULT 0,
    worldeffectscale       float4  NOT NULL DEFAULT 0,
    attachedeffectscale    float4  NOT NULL DEFAULT 0,
    missilecollisionradius float4  NOT NULL DEFAULT 0,
    missilecollisionpush   float4  NOT NULL DEFAULT 0,
    missilecollisionraise  float4  NOT NULL DEFAULT 0,

    CONSTRAINT pk_dbc_creaturemodeldata PRIMARY KEY (id)
);
