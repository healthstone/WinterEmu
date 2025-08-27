DROP TABLE IF EXISTS dbc.dbc_skillraceclassinfo;

CREATE TABLE dbc.dbc_skillraceclassinfo
(
    id             integer NOT NULL DEFAULT 0,
    skillid        integer NOT NULL DEFAULT 0,
    racemask       integer NOT NULL DEFAULT 0,
    classmask      integer NOT NULL DEFAULT 0,
    flags          integer NOT NULL DEFAULT 0,
    minlevel       integer NOT NULL DEFAULT 0,
    skilltierid    integer NOT NULL DEFAULT 0,
    skillcostindex integer NOT NULL DEFAULT 0,
    CONSTRAINT dbc_skillraceclassinfo_pkey PRIMARY KEY (id)
);
