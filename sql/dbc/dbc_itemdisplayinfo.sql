DROP TABLE IF EXISTS dbc.dbc_itemdisplayinfo;

CREATE TABLE dbc.dbc_itemdisplayinfo
(
    id                  integer PRIMARY KEY DEFAULT 0,
    model_name_1        text,
    model_name_2        text,
    model_texture_1     text,
    model_texture_2     text,
    inventory_icon_1    text,
    inventory_icon_2    text,
    geoset_group_1      integer NOT NULL    DEFAULT 0,
    geoset_group_2      integer NOT NULL    DEFAULT 0,
    geoset_group_3      integer NOT NULL    DEFAULT 0,
    flags               integer NOT NULL    DEFAULT 0,
    spell_visual_id     integer NOT NULL    DEFAULT 0,
    group_sound_index   integer NOT NULL    DEFAULT 0,
    helmet_geoset_vis_1 integer NOT NULL    DEFAULT 0,
    helmet_geoset_vis_2 integer NOT NULL    DEFAULT 0,
    texture_1           text,
    texture_2           text,
    texture_3           text,
    texture_4           text,
    texture_5           text,
    texture_6           text,
    texture_7           text,
    texture_8           text,
    item_visual         integer NOT NULL    DEFAULT 0,
    particle_color_id   integer NOT NULL    DEFAULT 0
);
