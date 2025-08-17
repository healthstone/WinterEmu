DROP TABLE IF EXISTS relay_server.character_account_data;

CREATE TABLE relay_server.character_account_data
(
    guid UUID     NOT NULL,
    type SMALLINT NOT NULL,
    time INTEGER  NOT NULL,
    data BYTEA    NOT NULL,
    PRIMARY KEY (guid, type),
    CONSTRAINT type_range CHECK (type BETWEEN 0 AND 7),
    CONSTRAINT time_non_negative CHECK (time >= 0)
);

-- Индексы для оптимизации запросов
CREATE INDEX idx_character_data_guid ON relay_server.character_account_data (guid);
CREATE INDEX idx_character_data_type ON relay_server.character_account_data (type);