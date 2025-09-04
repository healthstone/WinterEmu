DROP TABLE IF EXISTS dbc.dbc_creaturespelldata;

CREATE TABLE dbc.dbc_creaturespelldata
(
    id             integer NOT NULL DEFAULT 0,
    spells_1       integer NOT NULL DEFAULT 0,
    spells_2       integer NOT NULL DEFAULT 0,
    spells_3       integer NOT NULL DEFAULT 0,
    spells_4       integer NOT NULL DEFAULT 0,
    availability_1 integer NOT NULL DEFAULT 0,
    availability_2 integer NOT NULL DEFAULT 0,
    availability_3 integer NOT NULL DEFAULT 0,
    availability_4 integer NOT NULL DEFAULT 0,
    CONSTRAINT pk_dbc_creaturespelldata PRIMARY KEY (id)
);
