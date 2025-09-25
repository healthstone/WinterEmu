DROP TABLE IF EXISTS dbc.dbc_spellcategory;

CREATE TABLE dbc.dbc_spellcategory
(
    id    INTEGER PRIMARY KEY DEFAULT 0,
    flags INTEGER NOT NULL    DEFAULT 0
);
