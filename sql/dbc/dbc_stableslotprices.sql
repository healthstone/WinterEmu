DROP TABLE IF EXISTS dbc.dbc_stableslotprices;

CREATE TABLE dbc.dbc_stableslotprices
(
    id   INTEGER PRIMARY KEY DEFAULT 0,
    cost INTEGER NOT NULL DEFAULT 0
);

INSERT INTO dbc.dbc_stableslotprices (ID, Cost) VALUES(1, 500);
INSERT INTO dbc.dbc_stableslotprices (ID, Cost) VALUES(2, 50000);
INSERT INTO dbc.dbc_stableslotprices (ID, Cost) VALUES(3, 500000);
INSERT INTO dbc.dbc_stableslotprices (ID, Cost) VALUES(4, 1500000);