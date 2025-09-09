DROP TABLE IF EXISTS dbc.dbc_movie;

CREATE TABLE dbc.dbc_movie
(
    id       integer PRIMARY KEY DEFAULT 0,
    filename text,
    volume   integer NOT NULL    DEFAULT 0
);

INSERT INTO dbc.dbc_movie (id, filename, volume)
VALUES (1, 'Interface\\Cinematics\\Logo', 100),
       (2, 'Interface\\Cinematics\\WOW_Intro', 100),
       (14, '', 100),
       (16, '', 100);
