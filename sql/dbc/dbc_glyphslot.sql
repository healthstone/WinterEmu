DROP TABLE IF EXISTS dbc.dbc_glyphslot;

CREATE TABLE dbc.dbc_glyphslot
(
    id      integer PRIMARY KEY DEFAULT 0,
    type    integer NOT NULL    DEFAULT 0,
    tooltip integer NOT NULL    DEFAULT 0
);

INSERT INTO dbc.dbc_glyphslot (id, type, tooltip)
VALUES (1, 4, 0),
       (2, 2, 0),
       (3, 3, 0),
       (4, 0, 0),
       (21, 0, 1),
       (22, 1, 2),
       (23, 1, 3),
       (24, 0, 4),
       (25, 1, 5),
       (26, 0, 6);
