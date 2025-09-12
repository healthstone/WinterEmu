DROP TABLE IF EXISTS dbc.dbc_questfactionreward;

-- Создание таблицы в PostgreSQL
CREATE TABLE dbc.dbc_questfactionreward
(
    id            integer PRIMARY KEY DEFAULT 0,
    difficulty_1  integer NOT NULL    DEFAULT 0,
    difficulty_2  integer NOT NULL    DEFAULT 0,
    difficulty_3  integer NOT NULL    DEFAULT 0,
    difficulty_4  integer NOT NULL    DEFAULT 0,
    difficulty_5  integer NOT NULL    DEFAULT 0,
    difficulty_6  integer NOT NULL    DEFAULT 0,
    difficulty_7  integer NOT NULL    DEFAULT 0,
    difficulty_8  integer NOT NULL    DEFAULT 0,
    difficulty_9  integer NOT NULL    DEFAULT 0,
    difficulty_10 integer NOT NULL    DEFAULT 0
);

-- Вставка данных
INSERT INTO dbc.dbc_questfactionreward
(id, difficulty_1, difficulty_2, difficulty_3, difficulty_4, difficulty_5, difficulty_6, difficulty_7, difficulty_8,
 difficulty_9, difficulty_10)
VALUES (1, 0, 10, 25, 75, 150, 250, 350, 500, 1000, 5),
       (2, 0, -10, -25, -75, -150, -250, -350, -500, -1000, -5);
