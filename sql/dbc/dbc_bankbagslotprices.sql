DROP TABLE IF EXISTS dbc.dbc_bankbagslotprices;

CREATE TABLE dbc.dbc_bankbagslotprices
(
    id   INTEGER NOT NULL DEFAULT 0 PRIMARY KEY,
    cost INTEGER NOT NULL DEFAULT 0
);

INSERT INTO dbc.dbc_bankbagslotprices (id, cost)
VALUES (1, 1000),
       (2, 10000),
       (3, 100000),
       (4, 250000),
       (5, 250000),
       (6, 250000),
       (7, 250000),
       (8, 999999999),
       (9, 999999999),
       (10, 999999999),
       (11, 999999999),
       (12, 999999999);
