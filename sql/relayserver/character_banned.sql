DROP TABLE IF EXISTS relay_server.character_banned;

CREATE TABLE relay_server.character_banned
(
    guid      UUID         NOT NULL, -- Global Unique Identifier
    bandate   INTEGER      NOT NULL DEFAULT 0,
    unbandate INTEGER      NOT NULL DEFAULT 0,
    bannedby  VARCHAR(50)  NOT NULL,
    banreason VARCHAR(255) NOT NULL,
    active    SMALLINT     NOT NULL DEFAULT 1,
    PRIMARY KEY (guid, bandate)
);