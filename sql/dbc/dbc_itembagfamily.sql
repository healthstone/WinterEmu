DROP TABLE IF EXISTS dbc.dbc_itembagfamily;

CREATE TABLE dbc.dbc_itembagfamily
(
    id             integer PRIMARY KEY DEFAULT 0,
    name_lang_enUS text,
    name_lang_enGB text,
    name_lang_koKR text,
    name_lang_frFR text,
    name_lang_deDE text,
    name_lang_enCN text,
    name_lang_zhCN text,
    name_lang_enTW text,
    name_lang_zhTW text,
    name_lang_esES text,
    name_lang_esMX text,
    name_lang_ruRU text,
    name_lang_ptPT text,
    name_lang_ptBR text,
    name_lang_itIT text,
    name_lang_Unk  text,
    name_lang_mask integer NOT NULL    DEFAULT 0
);

INSERT INTO dbc.dbc_itembagfamily (
    id, name_lang_enUS, name_lang_enGB, name_lang_koKR, name_lang_frFR, name_lang_deDE,
    name_lang_enCN, name_lang_zhCN, name_lang_enTW, name_lang_zhTW, name_lang_esES,
    name_lang_esMX, name_lang_ruRU, name_lang_ptPT, name_lang_ptBR, name_lang_itIT,
    name_lang_Unk, name_lang_mask
) VALUES
      (0, 'NONE', 'NONE', '없음', 'AUCUNE', 'NICHTS', '无', '无', '無', '無', 'NADA', 'NADA', 'НЕТ', '', '', '', '', 16712190),
      (1, 'Arrows', 'Arrows', '화살', 'Flèches', 'Pfeile', '箭矢', '箭矢', '箭矢', '箭矢', 'Flechas', 'Flechas', 'Стрелы', '', '', '', '', 16712190),
      (2, 'Bullets', 'Bullets', '탄약', 'Balles', 'Kugeln', '弹药', '弹药', '子彈', '子彈', 'Balas', 'Balas', 'Пули', '', '', '', '', 16712190),
      (3, 'Soul Shards', 'Soul Shards', '영혼의 조각', 'Fragments d''âme', 'Seelensplitter', '灵魂碎片', '灵魂碎片', '靈魂裂片', '靈魂裂片', 'Fragmentos de almas', 'Fragmentos de almas', 'Камни душ', '', '', '', '', 16712190),
      (4, 'Leatherworking Supplies', 'Leatherworking Supplies', '가죽세공용 물품', 'Fournitures de travail du cuir', 'Lederverarbeitungsbedarf', '制皮', '制皮', '製皮補給品', '製皮補給品', 'Suministros de peletería', 'Suministros de peletería', 'Товары для кожевников', '', '', '', '', 16712190),
      (5, 'Inscription Supplies', 'Inscription Supplies', '주문각인 물품', 'Fournitures de calligraphie', 'Inschriftenkundebedarf', '铭文材料', '铭文材料', '銘文補給品', '銘文補給品', 'Suministros de inscripción', 'Suministros de inscripción', 'Письменные принадлежности', '', '', '', '', 16712191),
      (6, 'Herbs', 'Herbs', '약초', 'Herbes', 'Kräuter', '草药', '草药', '草藥', '草藥', 'Hierbas', 'Hierbas', 'Травы', '', '', '', '', 16712190),
      (7, 'Enchanting Supplies', 'Enchanting Supplies', '마법부여 재료', 'Fournitures d''enchanteur', 'Verzauberkunstbedarf', '附魔材料', '附魔材料', '附魔補給品', '附魔補給品', 'Encantamiento', 'Encantamiento', 'Товары для наложения чар', '', '', '', '', 16712190),
      (8, 'Engineering Supplies', 'Engineering Supplies', '기계공학 용품', 'Fournitures d''ingénieur', 'Ingenieursbedarf', '工程学材料', '工程学材料', '工程補給品', '工程補給品', 'Ingeniería', 'Ingeniería', 'Инженерные материалы', '', '', '', '', 16712190),
      (9, 'Keys', 'Keys', '열쇠', 'Clés', 'Schlüssel', '钥匙', '钥匙', '鑰匙', '鑰匙', 'Llaves', 'Llaves', 'Ключи', '', '', '', '', 16712190),
      (10, 'Gems', 'Gems', '보석', 'Gemmes', 'Edelsteine', '宝石', '宝石', '寶石', '寶石', 'Gemas', 'Gemas', 'Самоцветы', '', '', '', '', 16712190),
      (11, 'Mining Supplies', 'Mining Supplies', '채광 용품', 'Fournitures de mineur', 'Bergbaubedarf', '矿物', '矿物', '採礦補給品', '採礦補給品', 'Suministros de minería', 'Suministros de minería', 'Товары для горного дела', '', '', '', '', 16712190),
      (12, 'Soulbound Equipment', 'Soulbound Equipment', '귀속 장비', 'Équipement lié', 'Seelengebundene Ausrüstung', '已绑定装备', '已绑定装备', '靈魂綁定物品', '靈魂綁定物品', 'Equipo ligado al alma', 'Equipo ligado al alma', 'Персональное снаряжение', '', '', '', '', 16712190),
      (13, 'Vanity Pets', 'Vanity Pets', '애완동물', 'Mascottes', 'Haustiere', '小宠物', '小宠物', '小寵物', '小寵物', 'Mascotas de vanidad', 'Mascotas de vanidad', 'Спутники', '', '', '', '', 16712190),
      (14, 'Currency Tokens', 'Currency Tokens', '교환 증표', 'Monnaies', 'Währungsmarken', '货币物品', '货币物品', '兌換通貨', '兌換通貨', 'Monedas', 'Monedas', 'Обменные жетоны', '', '', '', '', 16712190),
      (15, 'Quest Items', 'Quest Items', '퀘스트 아이템', 'Objets de quêtes', 'Questgegenstände', '任务物品', '任务物品', '任務物品', '任務物品', 'Objetos de misión', 'Objetos de misión', 'Предм. для заданий', '', '', '', '', 16712190);
