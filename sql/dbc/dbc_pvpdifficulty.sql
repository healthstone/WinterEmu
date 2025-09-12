DROP TABLE IF EXISTS dbc.dbc_pvpdifficulty;

CREATE TABLE dbc.dbc_pvpdifficulty
(
    id          integer PRIMARY KEY DEFAULT 0,
    map_id      integer NOT NULL    DEFAULT 0,
    range_index integer NOT NULL    DEFAULT 0,
    min_level   integer NOT NULL    DEFAULT 0,
    max_level   integer NOT NULL    DEFAULT 0,
    difficulty  integer NOT NULL    DEFAULT 0
);
