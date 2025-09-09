DROP TABLE IF EXISTS dbc.dbc_mapdifficulty;

CREATE TABLE dbc.dbc_mapdifficulty
(
    id                 integer PRIMARY KEY DEFAULT 0,
    map_id             integer NOT NULL    DEFAULT 0,
    difficulty         integer NOT NULL    DEFAULT 0,

    message_lang_en_us text,
    message_lang_en_gb text,
    message_lang_ko_kr text,
    message_lang_fr_fr text,
    message_lang_de_de text,
    message_lang_en_cn text,
    message_lang_zh_cn text,
    message_lang_en_tw text,
    message_lang_zh_tw text,
    message_lang_es_es text,
    message_lang_es_mx text,
    message_lang_ru_ru text,
    message_lang_pt_pt text,
    message_lang_pt_br text,
    message_lang_it_it text,
    message_lang_unk   text,

    message_lang_mask  bigint  NOT NULL    DEFAULT 0, -- int unsigned → bigint

    raid_duration      integer NOT NULL    DEFAULT 0,
    max_players        integer NOT NULL    DEFAULT 0,
    difficulty_string  text
);
