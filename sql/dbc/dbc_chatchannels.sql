DROP TABLE IF EXISTS dbc.dbc_chatchannels;

CREATE TABLE dbc.dbc_chatchannels
(
    ID                 integer NOT NULL DEFAULT 0 PRIMARY KEY,
    Flags              integer NOT NULL DEFAULT 0,
    FactionGroup       integer NOT NULL DEFAULT 0,

    Name_Lang_enUS     text,
    Name_Lang_enGB     text,
    Name_Lang_koKR     text,
    Name_Lang_frFR     text,
    Name_Lang_deDE     text,
    Name_Lang_enCN     text,
    Name_Lang_zhCN     text,
    Name_Lang_enTW     text,
    Name_Lang_zhTW     text,
    Name_Lang_esES     text,
    Name_Lang_esMX     text,
    Name_Lang_ruRU     text,
    Name_Lang_ptPT     text,
    Name_Lang_ptBR     text,
    Name_Lang_itIT     text,
    Name_Lang_Unk      text,
    Name_Lang_Mask     bigint  NOT NULL DEFAULT 0,

    Shortcut_Lang_enUS text,
    Shortcut_Lang_enGB text,
    Shortcut_Lang_koKR text,
    Shortcut_Lang_frFR text,
    Shortcut_Lang_deDE text,
    Shortcut_Lang_enCN text,
    Shortcut_Lang_zhCN text,
    Shortcut_Lang_enTW text,
    Shortcut_Lang_zhTW text,
    Shortcut_Lang_esES text,
    Shortcut_Lang_esMX text,
    Shortcut_Lang_ruRU text,
    Shortcut_Lang_ptPT text,
    Shortcut_Lang_ptBR text,
    Shortcut_Lang_itIT text,
    Shortcut_Lang_Unk  text,
    Shortcut_Lang_Mask bigint  NOT NULL DEFAULT 0
);

INSERT INTO dbc.dbc_chatchannels (
    id, flags, factiongroup,
    name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw,
    name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, name_lang_unk, name_lang_mask,
    shortcut_lang_enus, shortcut_lang_engb, shortcut_lang_kokr, shortcut_lang_frfr, shortcut_lang_dede, shortcut_lang_encn, shortcut_lang_zhcn, shortcut_lang_entw, shortcut_lang_zhtw,
    shortcut_lang_eses, shortcut_lang_esmx, shortcut_lang_ruru, shortcut_lang_ptpt, shortcut_lang_ptbr, shortcut_lang_itit, shortcut_lang_unk, shortcut_lang_mask
)
VALUES
    (1, 524291, 0,
     'General - %s', 'General - %s', '공개 - %s', 'Général - %s', 'Allgemein - %s', '综合 - %s', '综合 - %s', '綜合 - %s', '綜合 - %s',
     'General: %s', 'General: %s', 'Общий: %s', '', '', '', '', 16712190,
     'General', 'General', '공개', 'Général', 'Allgemein', '综合', '综合', '綜合', '綜合',
     'General', 'General', 'Общий', '', '', '', '', 16712190),

    (2, 59, 0,
     'Trade - %s', 'Trade - %s', '거래 - %s', 'Commerce - %s', 'Handel - %s', '交易 - %s', '交易 - %s', '交易 - %s', '交易 - %s',
     'Comercio: %s', 'Comercio: %s', 'Торговля: %s', '', '', '', '', 16712190,
     'Trade', 'Trade', '거래', 'Commerce', 'Handel', '交易', '交易', '交易', '交易',
     'Comercio', 'Comercio', 'Торговля', '', '', '', '', 16712190),

    (22, 65539, 0,
     'LocalDefense - %s', 'LocalDefense - %s', '수비 - %s', 'DéfenseLocale - %s', 'LokaleVerteidigung - %s', '本地防务 - %s', '本地防务 - %s', '本地防務 - %s', '本地防務 - %s',
     'Defensa local: %s', 'Defensa local: %s', 'Оборона: %s', '', '', '', '', 16712190,
     'LocalDefense', 'LocalDefense', '수비', 'DéfenseLocale', 'LokaleVerteidigung', '本地防务', '本地防务', '本地防務', '本地防務',
     'DefensaLocal', 'DefensaLocal', 'ОборонаЛокальный', '', '', '', '', 16712190),

    (23, 65540, 0,
     'WorldDefense', 'WorldDefense', '전쟁', 'DéfenseUniverselle', 'WeltVerteidigung', '世界防务', '世界防务', '世界防務', '世界防務',
     'Defensa general', 'Defensa general', 'Оборона: глобальный', '', '', '', '', 16712190,
     'WorldDefense', 'WorldDefense', '전쟁', 'DéfenseUniverselle', 'WeltVerteidigung', '世界防务', '世界防务', '世界防務', '世界防務',
     'DefensaGeneral', 'DefensaGeneral', 'ОборонаГлобальный', '', '', '', '', 16712190),

    (25, 131122, 0,
     'GuildRecruitment - %s', 'GuildRecruitment - %s', '길드모집 - %s', 'RecrutementDeGuilde - %s', 'Gildenrekrutierung - %s', '公会招募 - %s', '公会招募 - %s', '公會招募 - %s', '公會招募 - %s',
     'BuscaHermandad: %s', 'BuscaHermandad: %s', 'Гильдии: %s', '', '', '', '', 16712190,
     'GuildRecruitment', 'GuildRecruitment', '길드모집', 'RecrutementDeGuilde', 'Gildenrekrutierung', '公会招募', '公会招募', '公會招募', '公會招募',
     'BuscaHermandad', 'BuscaHermandad', 'Гильдии', '', '', '', '', 16712190),

    (26, 262201, 0,
     'LookingForGroup', 'LookingForGroup', '파티찾기', 'RechercheDeGroupe', 'SucheNachGruppe', '寻求组队', '寻求组队', '尋求組隊', '尋求組隊',
     'BuscarGrupo', 'BuscarGrupo', 'Поиск спутников', '', '', '', '', 16712190,
     'LookingForGroup', 'LookingForGroup', '파티찾기', 'RechercheDeGroupe', 'SucheNachGruppe', '寻求组队', '寻求组队', '尋求組隊', '尋求組隊',
     'BuscarGrupo', 'BuscarGrupo', 'ПоискСпутников', '', '', '', '', 16712190
    );
