DROP TABLE IF EXISTS dbc.dbc_gtchancetomeleecritbase;

CREATE TABLE dbc.dbc_gtchancetomeleecritbase
(
    id   integer PRIMARY KEY DEFAULT 0,
    data float4 NOT NULL     DEFAULT 0
);

INSERT INTO dbc.dbc_gtchancetomeleecritbase (id, data)
VALUES (1, 0.031891),
       (2, 0.032685),
       (3, -0.01532),
       (4, -0.00295),
       (5, 0.031765),
       (6, 0.031891),
       (7, 0.02922),
       (8, 0.03454),
       (9, 0.02622),
       (10, 0.2),
       (11, 0.074755);
