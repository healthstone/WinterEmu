DROP TABLE IF EXISTS dbc.dbc_itemextendedcost;

CREATE TABLE dbc.dbc_itemextendedcost
(
    id                   integer PRIMARY KEY DEFAULT 0,
    honor_points         integer NOT NULL DEFAULT 0,
    arena_points         integer NOT NULL DEFAULT 0,
    arena_bracket        integer NOT NULL DEFAULT 0,
    item_id_1            integer NOT NULL DEFAULT 0,
    item_id_2            integer NOT NULL DEFAULT 0,
    item_id_3            integer NOT NULL DEFAULT 0,
    item_id_4            integer NOT NULL DEFAULT 0,
    item_id_5            integer NOT NULL DEFAULT 0,
    item_count_1         integer NOT NULL DEFAULT 0,
    item_count_2         integer NOT NULL DEFAULT 0,
    item_count_3         integer NOT NULL DEFAULT 0,
    item_count_4         integer NOT NULL DEFAULT 0,
    item_count_5         integer NOT NULL DEFAULT 0,
    required_arena_rating integer NOT NULL DEFAULT 0,
    item_purchase_group  integer NOT NULL DEFAULT 0
);
