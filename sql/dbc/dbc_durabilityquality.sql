DROP TABLE IF EXISTS dbc.dbc_durabilityquality;

CREATE TABLE dbc.dbc_durabilityquality
(
    id   integer PRIMARY KEY DEFAULT 0,
    data float4 NOT NULL     DEFAULT 0
);

INSERT INTO dbc.dbc_durabilityquality (id, data)
VALUES (1, 1.0),
       (2, 0.6),
       (3, 1.0),
       (4, 0.8),
       (5, 1.0),
       (6, 1.0),
       (7, 1.2),
       (8, 1.25),
       (9, 1.44),
       (10, 2.5),
       (11, 1.728),
       (12, 3.0),
       (13, 0.0),
       (14, 0.0),
       (15, 1.2),
       (16, 1.25);
