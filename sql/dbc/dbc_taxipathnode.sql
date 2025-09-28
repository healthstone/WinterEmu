DROP TABLE IF EXISTS dbc.dbc_taxipathnode;
-- PostgreSQL DDL
CREATE TABLE dbc.dbc_taxipathnode
(
    id               INTEGER PRIMARY KEY DEFAULT 0,
    pathid           INTEGER NOT NULL    DEFAULT 0,
    nodeindex        INTEGER NOT NULL    DEFAULT 0,
    continentid      INTEGER NOT NULL    DEFAULT 0,
    locx             REAL    NOT NULL    DEFAULT 0,
    locy             REAL    NOT NULL    DEFAULT 0,
    locz             REAL    NOT NULL    DEFAULT 0,
    flags            INTEGER NOT NULL    DEFAULT 0,
    delay            INTEGER NOT NULL    DEFAULT 0,
    arrivaleventid   INTEGER NOT NULL    DEFAULT 0,
    departureeventid INTEGER NOT NULL    DEFAULT 0
);
