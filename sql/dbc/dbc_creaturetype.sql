DROP TABLE IF EXISTS dbc.dbc_creaturetype;

CREATE TABLE dbc.dbc_creaturetype
(
    id             integer NOT NULL DEFAULT 0,
    name_lang_enus text,
    name_lang_engb text,
    name_lang_kokr text,
    name_lang_frfr text,
    name_lang_dede text,
    name_lang_encn text,
    name_lang_zhcn text,
    name_lang_entw text,
    name_lang_zhtw text,
    name_lang_eses text,
    name_lang_esmx text,
    name_lang_ruru text,
    name_lang_ptpt text,
    name_lang_ptbr text,
    name_lang_itit text,
    name_lang_unk  text,
    name_lang_mask bigint  NOT NULL DEFAULT 0,
    flags          integer NOT NULL DEFAULT 0,
    CONSTRAINT pk_dbc_creaturetype PRIMARY KEY (id)
);

INSERT INTO dbc.dbc_creaturetype (
    id, name_lang_enus, name_lang_engb, name_lang_kokr,
    name_lang_frfr, name_lang_dede, name_lang_encn, name_lang_zhcn,
    name_lang_entw, name_lang_zhtw, name_lang_eses, name_lang_esmx,
    name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit,
    name_lang_unk, name_lang_mask, flags
) VALUES
      (1,  'Beast',       'Beast',       '야수',        'Bête',              'Wildtier',       '野兽',     '野兽',     '野獸',     '野獸',     'Bestia',        'Bestia',        'Животное',    '', '', '', '', 16712190, 0),
      (2,  'Dragonkin',   'Dragonkin',   '용족',        'Draconien',         'Drachkin',       '龙类',     '龙类',     '龍類',     '龍類',     'Dragonante',    'Dragonante',    'Дракон',      '', '', '', '', 16712190, 0),
      (3,  'Demon',       'Demon',       '악마',        'Démon',             'Dämon',          '恶魔',     '恶魔',     '惡魔',     '惡魔',     'Demonio',       'Demonio',       'Демон',       '', '', '', '', 16712190, 0),
      (4,  'Elemental',   'Elemental',   '정령',        'Elémentaire',       'Elementar',      '元素生物', '元素生物', '元素生物', '元素生物', 'Elemental',     'Elemental',     'Элементаль',  '', '', '', '', 16712190, 0),
      (5,  'Giant',       'Giant',       '거인',        'Géant',             'Riese',          '巨人',     '巨人',     '巨人',     '巨人',     'Gigante',       'Gigante',       'Великан',     '', '', '', '', 16712190, 0),
      (6,  'Undead',      'Undead',      '언데드',      'Mort-vivant',       'Untoter',        '亡灵',     '亡灵',     '不死族',   '不死族',   'No-muerto',     'No-muerto',     'Нежить',      '', '', '', '', 16712190, 0),
      (7,  'Humanoid',    'Humanoid',    '인간형',      'Humanoïde',         'Humanoid',       '人型生物', '人型生物', '人型生物', '人型生物', 'Humanoide',     'Humanoide',     'Гуманоид',    '', '', '', '', 16712190, 0),
      (8,  'Critter',     'Critter',     '동물',        'Bestiole',          'Tier',           '小动物',   '小动物',   '小動物',   '小動物',   'Alimaña',       'Alimaña',       'Существо',    '', '', '', '', 16712190, 1),
      (9,  'Mechanical',  'Mechanical',  '기계',        'Machine',           'Mechanisch',     '机械',     '机械',     '機械',     '機械',     'Mecánico',      'Mecánico',      'Механизм',    '', '', '', '', 16712190, 0),
      (10, 'Not specified','Not specified','기타',      'Non spécifié',      'Nicht spezifiziert','未指定', '未指定',   '不明',     '不明',     'Sin especificar','Sin especificar','Не указано',  '', '', '', '', 16712190, 0),
      (11, 'Totem',       'Totem',       '토템',        'Totem',             'Totem',          '图腾',     '图腾',     '圖騰',     '圖騰',     'Tótem',         'Tótem',         'Тотем',       '', '', '', '', 16712190, 0),
      (12, 'Non-combat Pet','Non-combat Pet','애완동물', 'Familier pacifique','Haustier',       '非战斗宠物','非战斗宠物','非戰鬥寵物','非戰鬥寵物','Mascota mansa','Mascota mansa','Спутник',     '', '', '', '', 16712190, 1),
      (13, 'Gas Cloud',   'Gas Cloud',   '가스',        'Nuage de gaz',      'Gaswolke',       '气体云雾', '气体云雾', '氣體雲',   '氣體雲',   'Nube de gas',   'Nube de gas',   'Облако газа', '', '', '', '', 16712190, 1);
