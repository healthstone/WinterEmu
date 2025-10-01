DROP TABLE IF EXISTS dbc.dbc_lfgdungeonexpansion;

CREATE TABLE dbc.dbc_lfgdungeonexpansion
(
    id               INTEGER PRIMARY KEY DEFAULT 0,
    lfg_id           INTEGER             DEFAULT 0,
    expansion_level  INTEGER             DEFAULT 0,
    random_id        INTEGER             DEFAULT 0,
    hard_level_min   INTEGER             DEFAULT 0,
    hard_level_max   INTEGER             DEFAULT 0,
    target_level_min INTEGER             DEFAULT 0,
    target_level_max INTEGER             DEFAULT 0
);

INSERT INTO dbc.dbc_lfgdungeonexpansion (id, lfg_id, expansion_level, random_id, hard_level_min, hard_level_max, target_level_min, target_level_max)
VALUES (38, 2, 0, 258, 57, 60, 59, 60),
       (36, 30, 0, 258, 47, 60, 49, 52),
       (46, 32, 0, 258, 55, 60, 57, 60),
       (53, 34, 0, 258, 53, 60, 55, 58),
       (54, 36, 0, 258, 56, 60, 58, 60),
       (37, 38, 0, 258, 57, 60, 59, 60),
       (49, 40, 0, 258, 57, 60, 59, 60),
       (95, 136, 1, 259, 58, 70, 59, 62),
       (57, 137, 1, 259, 59, 70, 61, 63),
       (88, 138, 1, 259, 67, 70, 69, 70),
       (110, 140, 1, 259, 60, 70, 62, 64),
       (66, 147, 1, 259, 67, 70, 69, 70),
       (107, 148, 1, 259, 62, 70, 64, 66),
       (108, 149, 1, 259, 63, 70, 65, 67),
       (109, 150, 1, 259, 65, 70, 67, 68),
       (92, 151, 1, 259, 67, 70, 69, 70),
       (65, 170, 1, 259, 64, 70, 66, 68),
       (76, 171, 1, 259, 68, 70, 70, 70),
       (61, 172, 1, 259, 67, 70, 69, 70),
       (97, 173, 1, 259, 67, 70, 69, 70),
       (98, 174, 1, 259, 68, 70, 70, 70),
       (87, 198, 1, 259, 68, 70, 70, 70),
       (201, 258, 0, 0, 15, 60, 15, 60),
       (200, 258, 1, 0, 15, 58, 15, 58),
       (199, 258, 2, 0, 15, 58, 15, 58),
       (202, 259, 1, 0, 59, 70, 59, 70),
       (203, 259, 2, 0, 59, 68, 59, 68),
       (219, 274, 0, 258, 57, 60, 59, 60),
       (220, 276, 0, 258, 51, 60, 53, 56);
