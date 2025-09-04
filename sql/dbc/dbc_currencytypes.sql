DROP TABLE IF EXISTS dbc.dbc_currencytypes;

CREATE TABLE dbc.dbc_currencytypes
(
    id         integer NOT NULL DEFAULT 0,
    itemid     integer NOT NULL DEFAULT 0,
    categoryid integer NOT NULL DEFAULT 0,
    bitindex   integer NOT NULL DEFAULT 0,
    CONSTRAINT pk_dbc_currencytypes PRIMARY KEY (id)
);

INSERT INTO dbc.dbc_currencytypes (id, itemid, categoryid, bitindex)
VALUES (1, 37711, 1, 1),
       (2, 37742, 1, 2),
       (42, 29434, 1, 7),
       (61, 41596, 1, 8),
       (81, 43016, 1, 9),
       (241, 44990, 1, 25),
       (103, 43307, 2, 12),
       (104, 43308, 2, 13),
       (121, 20560, 2, 14),
       (122, 20559, 2, 15),
       (123, 29024, 2, 16),
       (124, 42425, 2, 17),
       (125, 20558, 2, 18),
       (126, 43589, 2, 19),
       (161, 43228, 2, 21),
       (181, 44209, 2, 22),
       (201, 37836, 2, 23),
       (321, 47395, 2, 28),
       (4, 38644, 3, 3),
       (101, 40752, 22, 10),
       (102, 40753, 22, 11),
       (221, 45624, 22, 24),
       (301, 47241, 22, 27),
       (341, 49426, 22, 29),
       (22, 41749, 24, 5),
       (141, 43949, 2089878896, 20);
