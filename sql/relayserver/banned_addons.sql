DROP TABLE IF EXISTS relay_server.banned_addons;

CREATE TABLE relay_server.banned_addons
(
    Id        SERIAL PRIMARY KEY,
    Name      VARCHAR(255) COLLATE "C" NOT NULL,
    Version   VARCHAR(255) COLLATE "C" NOT NULL DEFAULT '',
    Timestamp TIMESTAMP                NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE UNIQUE INDEX idx_name_ver ON relay_server.banned_addons (Name, Version);