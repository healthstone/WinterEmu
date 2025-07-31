DROP TABLE IF EXISTS auth_server.realmcharacters;

CREATE TABLE auth_server.realmcharacters
(
    realmid  int4 DEFAULT 0 NOT NULL,
    acctid   uuid           NOT NULL,
    numchars int2 DEFAULT 0 NOT NULL,
    CONSTRAINT realmcharacters_pkey PRIMARY KEY (realmid, acctid)
);