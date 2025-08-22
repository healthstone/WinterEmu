DROP TABLE IF EXISTS auth_server.accounts;

CREATE TABLE auth_server.accounts
(
    id               UUID PRIMARY KEY      DEFAULT gen_random_uuid(),
    username         VARCHAR(32)  NOT NULL DEFAULT '',
    salt             BYTEA        NOT NULL CHECK (octet_length(salt) = 32),
    verifier         BYTEA        NOT NULL CHECK (octet_length(verifier) = 32),
    session_key_auth BYTEA CHECK (octet_length(session_key_auth) = 40),
    session_key_bnet BYTEA CHECK (octet_length(session_key_bnet) <= 64),
    totp_secret      BYTEA CHECK (octet_length(totp_secret) <= 128),
    email            VARCHAR(255) NOT NULL DEFAULT '',
    reg_mail         VARCHAR(255) NOT NULL DEFAULT '',
    joindate         TIMESTAMP    NOT NULL DEFAULT CURRENT_TIMESTAMP,
    last_ip          VARCHAR(15)  NOT NULL DEFAULT '127.0.0.1',
    last_attempt_ip  VARCHAR(15)  NOT NULL DEFAULT '127.0.0.1',
    failed_logins    INTEGER      NOT NULL DEFAULT 0 CHECK (failed_logins >= 0),
    locked           SMALLINT     NOT NULL DEFAULT 0 CHECK (locked >= 0 AND locked <= 1),
    lock_country     VARCHAR(2)   NOT NULL DEFAULT '00',
    last_login       TIMESTAMP,
    online           SMALLINT     NOT NULL DEFAULT 0 CHECK (online >= 0 AND online <= 1),
    expansion        SMALLINT     NOT NULL DEFAULT 2 CHECK (expansion >= 0),
    mutetime         BIGINT       NOT NULL DEFAULT 0,
    mutereason       VARCHAR(255) NOT NULL DEFAULT '',
    muteby           VARCHAR(50)  NOT NULL DEFAULT '',
    locale           SMALLINT     NOT NULL DEFAULT 0 CHECK (locale >= 0),
    os               VARCHAR(3)   NOT NULL DEFAULT '',
    timezone_offset  SMALLINT     NOT NULL DEFAULT 0,
    recruiter        INTEGER      NOT NULL DEFAULT 0 CHECK (recruiter >= 0),
    coins            BIGINT       NOT NULL DEFAULT 0,
    CONSTRAINT accounts_username_key UNIQUE (username)
);

COMMENT ON TABLE auth_server.accounts IS 'Account System';

-- example for account with login=1 and password=1
INSERT INTO auth_server.accounts (username, salt, verifier, email, joindate)
VALUES ('1',
        decode('0A3E57EEA85D222817B72F5A6299B642D56DB174F522FF0F8C72172D1223AE63', 'hex'),
        decode('D08A4D3949264CFA1536E41EE8E0FCA3000D07B8BF3DCC1D524AA10535723689', 'hex'),
        'test@gmail.com',
        '2025-05-01 23:11:58');