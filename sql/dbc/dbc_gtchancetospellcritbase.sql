DROP TABLE IF EXISTS dbc.dbc_gtchancetospellcritbase;

CREATE TABLE dbc.dbc_gtchancetospellcritbase
(
    id   integer PRIMARY KEY DEFAULT 0,
    data float4 NOT NULL     DEFAULT 0
);

INSERT INTO dbc.dbc_gtchancetospellcritbase (id, data)
VALUES (1, 0.0),
       (2, 0.033355),
       (3, 0.03602),
       (4, 0.0),
       (5, 0.012375),
       (6, 0.0),
       (7, 0.02201),
       (8, 0.009075),
       (9, 0.017),
       (10, 0.2),
       (11, 0.018515);
