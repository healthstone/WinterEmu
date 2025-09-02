DROP TABLE IF EXISTS dbc.dbc_creaturedisplayinfoextra;

CREATE TABLE dbc.dbc_creaturedisplayinfoextra
(
    id                integer PRIMARY KEY DEFAULT 0,
    displayraceid     integer NOT NULL    DEFAULT 0,
    displaysexid      integer NOT NULL    DEFAULT 0,
    skinid            integer NOT NULL    DEFAULT 0,
    faceid            integer NOT NULL    DEFAULT 0,
    hairstyleid       integer NOT NULL    DEFAULT 0,
    haircolorid       integer NOT NULL    DEFAULT 0,
    facialhairid      integer NOT NULL    DEFAULT 0,
    npcitemdisplay_1  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_2  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_3  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_4  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_5  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_6  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_7  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_8  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_9  integer NOT NULL    DEFAULT 0,
    npcitemdisplay_10 integer NOT NULL    DEFAULT 0,
    npcitemdisplay_11 integer NOT NULL    DEFAULT 0,
    flags             integer NOT NULL    DEFAULT 0,
    bakename          text
);
