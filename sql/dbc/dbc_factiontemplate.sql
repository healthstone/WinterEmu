DROP TABLE IF EXISTS dbc.dbc_factiontemplate;

CREATE TABLE dbc.dbc_factiontemplate
(
    id           integer PRIMARY KEY DEFAULT 0,
    faction      integer NOT NULL    DEFAULT 0,
    flags        integer NOT NULL    DEFAULT 0,
    factiongroup integer NOT NULL    DEFAULT 0,
    friendgroup  integer NOT NULL    DEFAULT 0,
    enemygroup   integer NOT NULL    DEFAULT 0,
    enemies_1    integer NOT NULL    DEFAULT 0,
    enemies_2    integer NOT NULL    DEFAULT 0,
    enemies_3    integer NOT NULL    DEFAULT 0,
    enemies_4    integer NOT NULL    DEFAULT 0,
    friend_1     integer NOT NULL    DEFAULT 0,
    friend_2     integer NOT NULL    DEFAULT 0,
    friend_3     integer NOT NULL    DEFAULT 0,
    friend_4     integer NOT NULL    DEFAULT 0
);
