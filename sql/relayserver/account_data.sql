DROP TABLE IF EXISTS relay_server.account_data;

CREATE TABLE relay_server.account_data
(
    account_id UUID     NOT NULL,
    type       SMALLINT NOT NULL,
    time       INTEGER  NOT NULL,
    data       BYTEA    NOT NULL,
    PRIMARY KEY (account_id, type),
    CONSTRAINT type_range CHECK (type BETWEEN 0 AND 7),
    CONSTRAINT time_non_negative CHECK (time >= 0)
);

-- Создаем индексы для оптимизации запросов
CREATE INDEX idx_account_data_account ON relay_server.account_data (account_id);
CREATE INDEX idx_account_data_type ON relay_server.account_data (type);