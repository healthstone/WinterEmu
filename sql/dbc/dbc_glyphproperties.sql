DROP TABLE IF EXISTS dbc.dbc_glyphproperties;

CREATE TABLE dbc.dbc_glyphproperties
(
    id             integer PRIMARY KEY DEFAULT 0,
    spellid        integer NOT NULL    DEFAULT 0,
    glyphslotflags integer NOT NULL    DEFAULT 0,
    spelliconid    integer NOT NULL    DEFAULT 0
);
