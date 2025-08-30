DROP TABLE IF EXISTS dbc.dbc_bannedaddons;

CREATE TABLE dbc.dbc_bannedaddons
(
    id           INTEGER NOT NULL DEFAULT 0,
    namemd5_1    BIGINT  NOT NULL DEFAULT 0,
    namemd5_2    BIGINT  NOT NULL DEFAULT 0,
    namemd5_3    BIGINT  NOT NULL DEFAULT 0,
    namemd5_4    BIGINT  NOT NULL DEFAULT 0,
    versionmd5_1 BIGINT  NOT NULL DEFAULT 0,
    versionmd5_2 BIGINT  NOT NULL DEFAULT 0,
    versionmd5_3 BIGINT  NOT NULL DEFAULT 0,
    versionmd5_4 BIGINT  NOT NULL DEFAULT 0,
    lastmodified INTEGER NOT NULL DEFAULT 0,
    flags        INTEGER NOT NULL DEFAULT 0,
    CONSTRAINT pk_dbc_bannedaddons PRIMARY KEY (id)
);
