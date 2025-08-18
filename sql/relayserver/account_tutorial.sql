DROP TABLE IF EXISTS relay_server.account_tutorial;

CREATE TABLE relay_server.account_tutorial
(
    account_id UUID    NOT NULL PRIMARY KEY,
    tut0       INTEGER NOT NULL DEFAULT 0 CHECK (tut0 >= 0),
    tut1       INTEGER NOT NULL DEFAULT 0 CHECK (tut1 >= 0),
    tut2       INTEGER NOT NULL DEFAULT 0 CHECK (tut2 >= 0),
    tut3       INTEGER NOT NULL DEFAULT 0 CHECK (tut3 >= 0),
    tut4       INTEGER NOT NULL DEFAULT 0 CHECK (tut4 >= 0),
    tut5       INTEGER NOT NULL DEFAULT 0 CHECK (tut5 >= 0),
    tut6       INTEGER NOT NULL DEFAULT 0 CHECK (tut6 >= 0),
    tut7       INTEGER NOT NULL DEFAULT 0 CHECK (tut7 >= 0)
);

COMMENT ON TABLE relay_server.account_tutorial IS 'Account tutorial flags storage';
COMMENT ON COLUMN relay_server.account_tutorial.account_id IS 'Account Identifier (UUID)';
COMMENT ON COLUMN relay_server.account_tutorial.tut0 IS 'Tutorial flag 0';
COMMENT ON COLUMN relay_server.account_tutorial.tut1 IS 'Tutorial flag 1';
COMMENT ON COLUMN relay_server.account_tutorial.tut2 IS 'Tutorial flag 2';
COMMENT ON COLUMN relay_server.account_tutorial.tut3 IS 'Tutorial flag 3';
COMMENT ON COLUMN relay_server.account_tutorial.tut4 IS 'Tutorial flag 4';
COMMENT ON COLUMN relay_server.account_tutorial.tut5 IS 'Tutorial flag 5';
COMMENT ON COLUMN relay_server.account_tutorial.tut6 IS 'Tutorial flag 6';
COMMENT ON COLUMN relay_server.account_tutorial.tut7 IS 'Tutorial flag 7';