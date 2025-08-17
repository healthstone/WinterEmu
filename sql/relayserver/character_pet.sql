DROP TABLE IF EXISTS relay_server.character_pet;

CREATE TABLE relay_server.character_pet
(
    id             UUID PRIMARY KEY     DEFAULT gen_random_uuid(),
    entry          INTEGER     NOT NULL DEFAULT 0,
    owner          UUID        NOT NULL, -- Account/Character Identifier

    modelid        INTEGER              DEFAULT 0,
    CreatedBySpell INTEGER     NOT NULL DEFAULT 0,
    PetType        SMALLINT    NOT NULL DEFAULT 0,
    level          SMALLINT    NOT NULL DEFAULT 1,
    exp            INTEGER     NOT NULL DEFAULT 0,
    Reactstate     SMALLINT    NOT NULL DEFAULT 0,

    name           VARCHAR(21) NOT NULL DEFAULT 'Pet',
    renamed        SMALLINT    NOT NULL DEFAULT 0,
    slot           SMALLINT    NOT NULL DEFAULT 0,

    curhealth      INTEGER     NOT NULL DEFAULT 1,
    curmana        INTEGER     NOT NULL DEFAULT 0,
    curhappiness   INTEGER     NOT NULL DEFAULT 0,

    savetime       INTEGER     NOT NULL DEFAULT 0,

    abdata         TEXT
);

-- Индексы
CREATE INDEX idx_owner ON relay_server.character_pet (owner);
CREATE INDEX idx_slot ON relay_server.character_pet (slot);