DROP TABLE IF EXISTS relay_server.guild_member;

CREATE TABLE relay_server.guild_member
(
    guildid SERIAL PRIMARY KEY,                  -- Изменено на SERIAL (uint32)
    guid    INTEGER NOT NULL,                    -- Изменено на INTEGER (ссылка на characters.guid)
    rank    SMALLINT NOT NULL,
    pnote   VARCHAR(31) NOT NULL DEFAULT '',
    offnote VARCHAR(31) NOT NULL DEFAULT ''
);

COMMENT ON TABLE relay_server.guild_member IS 'Guild System';

-- Индексы для производительности
CREATE UNIQUE INDEX idx_guild_member_guid ON relay_server.guild_member (guid);
CREATE INDEX idx_guild_member_guildid ON relay_server.guild_member (guildid);
CREATE INDEX idx_guild_member_guildid_rank ON relay_server.guild_member (guildid, rank);