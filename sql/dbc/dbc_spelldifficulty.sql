DROP TABLE IF EXISTS dbc.dbc_spelldifficulty;

CREATE TABLE dbc.dbc_spelldifficulty
(
    id                   INTEGER PRIMARY KEY DEFAULT 0,
    difficulty_spell_id_1 INTEGER NOT NULL DEFAULT 0,
    difficulty_spell_id_2 INTEGER NOT NULL DEFAULT 0,
    difficulty_spell_id_3 INTEGER NOT NULL DEFAULT 0,
    difficulty_spell_id_4 INTEGER NOT NULL DEFAULT 0
);
