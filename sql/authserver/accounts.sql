DROP TABLE IF EXISTS auth_server.accounts;

CREATE TABLE auth_server.accounts
(
    id               UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    username         varchar(50) NOT NULL,
    salt             bytea       NOT NULL,
    verifier         bytea       NOT NULL,
    email            varchar(100) NULL,
    created_at       timestamp        DEFAULT CURRENT_TIMESTAMP NULL,
    session_key_auth bytea NULL,
    last_ip          varchar(15)      DEFAULT '127.0.0.1':: character varying NOT NULL,
    last_login       timestamp NULL,
    CONSTRAINT accounts_salt_check CHECK (octet_length(salt) = 32),
    CONSTRAINT accounts_session_key_auth_check CHECK (octet_length(session_key_auth) = 40 OR session_key_auth IS NULL),
    CONSTRAINT accounts_verifier_check CHECK (octet_length(verifier) = 32),
    CONSTRAINT accounts_username_key UNIQUE (username)
);

-- example for account with login=1 and password=1
INSERT INTO auth_server.accounts (username, salt, verifier, email, created_at)
VALUES ('1',
        decode('0A3E57EEA85D222817B72F5A6299B642D56DB174F522FF0F8C72172D1223AE63', 'hex'),
        decode('D08A4D3949264CFA1536E41EE8E0FCA3000D07B8BF3DCC1D524AA10535723689', 'hex'),
        'test@gmail.com',
        '2025-05-01 23:11:58');