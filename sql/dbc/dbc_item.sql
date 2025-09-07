DROP TABLE IF EXISTS dbc.dbc_item;

CREATE TABLE dbc.dbc_item
(
    id                        integer PRIMARY KEY DEFAULT 0,
    class_id                  integer NOT NULL    DEFAULT 0,
    subclass_id               integer NOT NULL    DEFAULT 0,
    sound_override_subclassid integer NOT NULL    DEFAULT 0,
    material                  integer NOT NULL    DEFAULT 0,
    display_info_id           integer NOT NULL    DEFAULT 0,
    inventory_type            integer NOT NULL    DEFAULT 0,
    sheathe_type              integer NOT NULL    DEFAULT 0
);
