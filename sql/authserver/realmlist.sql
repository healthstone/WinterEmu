DROP TABLE IF EXISTS auth_server.realmlist;

CREATE TABLE auth_server.realmlist
(
    id                     serial4                    NOT NULL,
    "name"                 varchar(32)                NOT NULL,
    address                varchar(255) DEFAULT '127.0.0.1':: character varying NOT NULL,
    local_address          varchar(255) DEFAULT '127.0.0.1':: character varying NOT NULL,
    local_subnet_mask      varchar(255) DEFAULT '255.255.255.0':: character varying NOT NULL,
    port                   int2         DEFAULT 8085  NOT NULL,
    icon                   int2         DEFAULT 0     NOT NULL,
    flag                   int2         DEFAULT 2     NOT NULL,
    timezone               int2         DEFAULT 0     NOT NULL,
    allowed_security_level int2         DEFAULT 0     NOT NULL,
    population             float4       DEFAULT 0     NOT NULL,
    gamebuild              int4         DEFAULT 12340 NOT NULL,
    CONSTRAINT realmlist_name_key UNIQUE (name),
    CONSTRAINT realmlist_pkey PRIMARY KEY (id)
);

INSERT INTO auth_server.realmlist (id, "name", address, local_address, local_subnet_mask, port, icon, flag, timezone,
                                   allowed_security_level, population, gamebuild)
VALUES (1, 'WinterRealm', '127.0.0.1', '127.0.0.1', '255.255.255.0', 8085, 8, 2, 1, 0, 0.0, 12340);