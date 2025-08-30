DROP TABLE IF EXISTS dbc.dbc_battlemasterlist;

CREATE TABLE dbc.dbc_battlemasterlist
(
    id                 INTEGER NOT NULL DEFAULT 0 PRIMARY KEY,
    mapid_1            INTEGER NOT NULL DEFAULT 0,
    mapid_2            INTEGER NOT NULL DEFAULT 0,
    mapid_3            INTEGER NOT NULL DEFAULT 0,
    mapid_4            INTEGER NOT NULL DEFAULT 0,
    mapid_5            INTEGER NOT NULL DEFAULT 0,
    mapid_6            INTEGER NOT NULL DEFAULT 0,
    mapid_7            INTEGER NOT NULL DEFAULT 0,
    mapid_8            INTEGER NOT NULL DEFAULT 0,
    instance_type      INTEGER NOT NULL DEFAULT 0,
    groups_allowed     INTEGER NOT NULL DEFAULT 0,

    name_lang_enus     TEXT,
    name_lang_engb     TEXT,
    name_lang_kokr     TEXT,
    name_lang_frfr     TEXT,
    name_lang_dede     TEXT,
    name_lang_encn     TEXT,
    name_lang_zhcn     TEXT,
    name_lang_entw     TEXT,
    name_lang_zhtw     TEXT,
    name_lang_eses     TEXT,
    name_lang_esmx     TEXT,
    name_lang_ruru     TEXT,
    name_lang_ptpt     TEXT,
    name_lang_ptbr     TEXT,
    name_lang_itit     TEXT,
    name_lang_unk      TEXT,
    name_lang_mask     BIGINT  NOT NULL DEFAULT 0,

    max_group_size     INTEGER NOT NULL DEFAULT 0,
    holiday_worldstate INTEGER NOT NULL DEFAULT 0,
    minlevel           INTEGER NOT NULL DEFAULT 0,
    maxlevel           INTEGER NOT NULL DEFAULT 0
);

INSERT INTO dbc.dbc_battlemasterlist (
    id, mapid_1, mapid_2, mapid_3, mapid_4, mapid_5, mapid_6, mapid_7, mapid_8,
    instance_type, groups_allowed,
    name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede,
    name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw,
    name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr,
    name_lang_itit, name_lang_unk, name_lang_mask,
    max_group_size, holiday_worldstate, minlevel, maxlevel
) VALUES
      (1, 30, -1, -1, -1, -1, -1, -1, -1, 3, 1, 'Alterac Valley', 'Alterac Valley', '알터랙 계곡', 'Vallée d''Alterac', 'Alteractal', '奥特兰克山谷', '奥特兰克山谷', '奧特蘭克山谷', '奧特蘭克山谷', 'Valle de Alterac', 'Valle de Alterac', 'Альтеракская долина', '', '', '', '', 16712190, 5, 1941, 51, 80),
      (2, 489, -1, -1, -1, -1, -1, -1, -1, 3, 1, 'Warsong Gulch', 'Warsong Gulch', '전쟁노래 협곡', 'Goulet des Chanteguerres', 'Kriegshymnenschlucht', '战歌峡谷', '战歌峡谷', '戰歌峽谷', '戰歌峽谷', 'Garganta Grito de Guerra', 'Garganta Grito de Guerra', 'Ущелье Песни Войны', '', '', '', '', 16712190, 10, 1942, 10, 80),
      (3, 529, -1, -1, -1, -1, -1, -1, -1, 3, 1, 'Arathi Basin', 'Arathi Basin', '아라시 분지', 'Bassin d''Arathi', 'Arathibecken', '阿拉希盆地', '阿拉希盆地', '阿拉希盆地', '阿拉希盆地', 'Cuenca de Arathi', 'Cuenca de Arathi', 'Низина Арати', '', '', '', '', 16712190, 15, 1943, 20, 80),
      (4, 559, -1, -1, -1, -1, -1, -1, -1, 4, 1, 'Nagrand Arena', 'Nagrand Arena', '나그란드 투기장', 'Arène de Nagrand', 'Arena von Nagrand', '纳格兰竞技场', '纳格兰竞技场', '納葛蘭競技場', '納葛蘭競技場', 'Arena de Nagrand', 'Arena de Nagrand', 'Арена Награнда', '', '', '', '', 16712190, 5, 0, 10, 80),
      (5, 562, -1, -1, -1, -1, -1, -1, -1, 4, 1, 'Blade''s Edge Arena', 'Blade''s Edge Arena', '칼날 산맥 투기장', 'Arène des Tranchantes', 'Arena des Schergrats', '刀锋竞技场', '刀锋竞技场', '劍刃競技場', '劍刃競技場', 'Arena Filospada', 'Arena Filospada', 'Арена Острогорья', '', '', '', '', 16712190, 5, 0, 10, 80),
      (6, 559, 562, 572, 617, 618, -1, -1, -1, 4, 1, 'All Arenas', 'All Arenas', '모든 투기장', 'Toutes les arènes', 'Alle Arenen', '所有竞技场', '所有竞技场', '所有競技場', '所有競技場', 'Todas las arenas', 'Todas las arenas', 'Все арены', '', '', '', '', 16712190, 5, 0, 10, 80),
      (7, 566, -1, -1, -1, -1, -1, -1, -1, 3, 1, 'Eye of the Storm', 'Eye of the Storm', '폭풍의 눈', 'L''Œil du cyclone', 'Auge des Sturms', '风暴之眼', '风暴之眼', '暴風之眼', '暴風之眼', 'Ojo de la Tormenta', 'Ojo de la Tormenta', 'Око Бури', '', '', '', '', 16712190, 15, 2851, 61, 80),
      (8, 572, -1, -1, -1, -1, -1, -1, -1, 4, 1, 'Ruins of Lordaeron', 'Ruins of Lordaeron', '로데론의 폐허', 'Ruines de Lordaeron', 'Ruinen von Lordaeron', '洛丹伦废墟', '洛丹伦废墟', '羅德隆廢墟', '羅德隆廢墟', 'Ruinas de Lordaeron', 'Ruinas de Lordaeron', 'Руины Лордерона', '', '', '', '', 16712190, 5, 0, 10, 80),
      (9, 607, -1, -1, -1, -1, -1, -1, -1, 3, 1, 'Strand of the Ancients', 'Strand of the Ancients', '고대의 해안', 'Rivage des Anciens', 'Strand der Uralten', '远古海滩', '远古海滩', '遠祖灘頭', '遠祖灘頭', 'Playa de los Ancestros', 'Playa de los Ancestros', 'Берег Древних', '', '', '', '', 16712190, 15, 3695, 71, 80),
      (10, 617, -1, -1, -1, -1, -1, -1, -1, 4, 1, 'Dalaran Sewers', 'Dalaran Sewers', '달라란 투기장', 'Egouts de Dalaran', 'Abwasserkanäle von Dalaran', '达拉然下水道', '达拉然下水道', '達拉然下水道', '達拉然下水道', 'Cloacas de Dalaran', 'Cloacas de Dalaran', 'Стоки Даларана', '', '', '', '', 16712190, 5, 0, 10, 80),
      (11, 618, -1, -1, -1, -1, -1, -1, -1, 4, 1, 'The Ring of Valor', 'The Ring of Valor', '용맹의 투기장', 'L''arène des valeureux', 'Der Ring der Ehre', '勇气竞技场', '勇气竞技场', '勇武之環', '勇武之環', 'El Círculo del Valor', 'El Círculo del Valor', 'Арена Доблести', '', '', '', '', 16712190, 5, 0, 10, 80),
      (30, 628, -1, -1, -1, -1, -1, -1, -1, 3, 1, 'Isle of Conquest', 'Isle of Conquest', '정복의 섬', 'Île des Conquérants', 'Insel der Eroberung', '征服之岛', '征服之岛', '征服之島', '征服之島', 'Isla de la Conquista', 'Isla de la Conquista', 'Остров Завоеваний', '', '', '', '', 16712190, 5, 4273, 71, 80),
      (32, 30, 489, 529, 566, 607, 628, -1, -1, 3, 1, 'Random Battleground', 'Random Battleground', '무작위 전장', 'Champ de bataille aléatoire', 'Zufälliges Schlachtfeld', '随机战场', '随机战场', '隨機戰場', '隨機戰場', 'Campo de batalla aleatorio', 'Campo de batalla aleatorio', 'Случайное поле боя', '', '', '', '', 16712190, 5, 0, 0, 0);

