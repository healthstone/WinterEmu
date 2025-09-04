DROP TABLE IF EXISTS dbc.dbc_durabilitycosts;

CREATE TABLE dbc.dbc_durabilitycosts
(
    id                    integer NOT NULL DEFAULT 0,

    weaponsubclasscost_1  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_2  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_3  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_4  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_5  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_6  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_7  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_8  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_9  integer NOT NULL DEFAULT 0,
    weaponsubclasscost_10 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_11 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_12 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_13 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_14 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_15 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_16 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_17 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_18 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_19 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_20 integer NOT NULL DEFAULT 0,
    weaponsubclasscost_21 integer NOT NULL DEFAULT 0,

    armorsubclasscost_1   integer NOT NULL DEFAULT 0,
    armorsubclasscost_2   integer NOT NULL DEFAULT 0,
    armorsubclasscost_3   integer NOT NULL DEFAULT 0,
    armorsubclasscost_4   integer NOT NULL DEFAULT 0,
    armorsubclasscost_5   integer NOT NULL DEFAULT 0,
    armorsubclasscost_6   integer NOT NULL DEFAULT 0,
    armorsubclasscost_7   integer NOT NULL DEFAULT 0,
    armorsubclasscost_8   integer NOT NULL DEFAULT 0,
    PRIMARY KEY (id)
);
