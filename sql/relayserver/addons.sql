DROP TABLE IF EXISTS relay_server.addons;

CREATE TABLE relay_server.addons
(
    name VARCHAR(120) COLLATE "C" NOT NULL DEFAULT '',
    crc  BIGINT                   NOT NULL DEFAULT 0,
    PRIMARY KEY (name)
);

COMMENT ON TABLE relay_server.addons IS 'Addons';

INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_AchievementUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_ArenaUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_AuctionUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_BarbershopUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_BattlefieldMinimap', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_BindingUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_Calendar', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_CombatLog', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_CombatText', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_DebugTools', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_GlyphUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_GMChatUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_GMSurveyUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_GuildBankUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_InspectUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_ItemSocketingUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_MacroUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_RaidUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_TalentUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_TimeManager', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_TokenUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_TradeSkillUI', 1276933997);
INSERT INTO relay_server.addons (name, crc) VALUES('Blizzard_TrainerUI', 1276933997);