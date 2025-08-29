DROP TABLE IF EXISTS dbc.dbc_areatrigger;

CREATE TABLE dbc.dbc_areatrigger
(
    id           INTEGER NOT NULL DEFAULT 0,
    continent_id INTEGER NOT NULL DEFAULT 0,
    x            FLOAT4  NOT NULL DEFAULT 0,
    y            FLOAT4  NOT NULL DEFAULT 0,
    z            FLOAT4  NOT NULL DEFAULT 0,
    radius       FLOAT4  NOT NULL DEFAULT 0,
    box_length   FLOAT4  NOT NULL DEFAULT 0,
    box_width    FLOAT4  NOT NULL DEFAULT 0,
    box_height   FLOAT4  NOT NULL DEFAULT 0,
    box_yaw      FLOAT4  NOT NULL DEFAULT 0,
    PRIMARY KEY (id)
);
