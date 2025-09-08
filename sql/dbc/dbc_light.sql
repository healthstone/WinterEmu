DROP TABLE IF EXISTS dbc.dbc_light;

CREATE TABLE dbc.dbc_light
(
    id              integer PRIMARY KEY DEFAULT 0,
    continent_id    integer NOT NULL DEFAULT 0,
    x               float4    NOT NULL DEFAULT 0,
    y               float4    NOT NULL DEFAULT 0,
    z               float4    NOT NULL DEFAULT 0,
    falloff_start   float4    NOT NULL DEFAULT 0,
    falloff_end     float4    NOT NULL DEFAULT 0,
    lightparamsid_1 integer NOT NULL DEFAULT 0,
    lightparamsid_2 integer NOT NULL DEFAULT 0,
    lightparamsid_3 integer NOT NULL DEFAULT 0,
    lightparamsid_4 integer NOT NULL DEFAULT 0,
    lightparamsid_5 integer NOT NULL DEFAULT 0,
    lightparamsid_6 integer NOT NULL DEFAULT 0,
    lightparamsid_7 integer NOT NULL DEFAULT 0,
    lightparamsid_8 integer NOT NULL DEFAULT 0
);
