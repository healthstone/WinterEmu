DROP TABLE IF EXISTS dbc.dbc_cinematicsequences;

CREATE TABLE dbc.dbc_cinematicsequences
(
    id       integer PRIMARY KEY DEFAULT 0,
    soundid  integer NOT NULL    DEFAULT 0,
    camera_1 integer NOT NULL    DEFAULT 0,
    camera_2 integer NOT NULL    DEFAULT 0,
    camera_3 integer NOT NULL    DEFAULT 0,
    camera_4 integer NOT NULL    DEFAULT 0,
    camera_5 integer NOT NULL    DEFAULT 0,
    camera_6 integer NOT NULL    DEFAULT 0,
    camera_7 integer NOT NULL    DEFAULT 0,
    camera_8 integer NOT NULL    DEFAULT 0
);

INSERT INTO dbc.dbc_cinematicsequences (id, soundid, camera_1, camera_2, camera_3, camera_4, camera_5, camera_6,
                                        camera_7, camera_8)
VALUES (1, 0, 1, 0, 0, 0, 0, 0, 0, 0),
       (2, 0, 2, 0, 0, 0, 0, 0, 0, 0),
       (21, 0, 235, 0, 0, 0, 0, 0, 0, 0),
       (41, 0, 234, 0, 0, 0, 0, 0, 0, 0),
       (61, 0, 122, 0, 0, 0, 0, 0, 0, 0),
       (81, 0, 142, 0, 0, 0, 0, 0, 0, 0),
       (101, 0, 162, 0, 0, 0, 0, 0, 0, 0),
       (121, 0, 182, 0, 0, 0, 0, 0, 0, 0),
       (141, 0, 202, 0, 0, 0, 0, 0, 0, 0),
       (161, 0, 224, 0, 0, 0, 0, 0, 0, 0),
       (162, 0, 243, 0, 0, 0, 0, 0, 0, 0),
       (163, 0, 244, 0, 0, 0, 0, 0, 0, 0),
       (164, 0, 245, 0, 0, 0, 0, 0, 0, 0),
       (165, 0, 246, 0, 0, 0, 0, 0, 0, 0);
