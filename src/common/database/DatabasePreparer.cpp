#include "DatabasePreparer.hpp"
#include "spdlog/fmt/bundled/format.h"
#include <cstdlib>

std::string DatabasePreparer::getAuthSchema() {
    return std::getenv("AUTH_SCHEMA") ? std::string(std::getenv("AUTH_SCHEMA")) : "auth_server";
}

std::string DatabasePreparer::getRelaySchema() {
    return std::getenv("RELAY_SCHEMA") ? std::string(std::getenv("RELAY_SCHEMA")) : "relay_server";
}

std::string DatabasePreparer::getDBCSchema() {
    return std::getenv("DBC_SCHEMA") ? std::string(std::getenv("DBC_SCHEMA")) : "dbc";
}

std::string DatabasePreparer::getWorldSchema() {
    return std::getenv("WORLD_SCHEMA") ? std::string(std::getenv("WORLD_SCHEMA")) : "world";
}

void DatabasePreparer::prepare_all(pqxx::connection &conn) {
    pqxx::work txn(conn);

    prepareAuthSchema(conn);
    prepareRelaySchema(conn);
    prepareDBCSchema(conn);
    prepareWorldSchema(conn);

    txn.commit();
}

void DatabasePreparer::prepareAuthSchema(pqxx::connection &conn) {
    std::string auth_schema = getAuthSchema();

    conn.prepare("SELECT_ACCOUNT_BY_USERNAME",
                 fmt::format("SELECT "
                             "id, username, salt, verifier, session_key_auth, session_key_bnet, "
                             "totp_secret, email, reg_mail, joindate, last_ip, last_attempt_ip, "
                             "failed_logins, locked, lock_country, last_login, online, expansion, "
                             "mutetime, mutereason, muteby, locale, os, timezone_offset, recruiter, coins "
                             "FROM {}.accounts WHERE username = $1", auth_schema));
    conn.prepare("SELECT_BUILD_INFO",
                 fmt::format("SELECT majorVersion, minorVersion, bugfixVersion, hotfixVersion, build FROM {}.build_info ORDER BY build ASC", auth_schema));
    conn.prepare("SELECT_BUILD_EXECUTABLE_HASH",
                 fmt::format("SELECT build, platform, executableHash FROM {}.build_executable_hash", auth_schema));
    conn.prepare("SELECT_REALMLIST",
                 fmt::format("SELECT id, name, address, local_address, local_subnet_mask, port, icon, flag, timezone, allowed_security_level, population, gamebuild FROM {}.realmlist WHERE flag <> 3 ORDER BY name", auth_schema));
    conn.prepare("SELECT_REALMLIST_BY_ID",
                 fmt::format("SELECT id, name, address, local_address, local_subnet_mask, port, icon, flag, timezone, allowed_security_level, population, gamebuild FROM {}.realmlist WHERE id = $1", auth_schema));
    conn.prepare("SELECT_REALM_CHARACTERS",
                 fmt::format("SELECT realmid, numchars FROM {}.realmcharacters WHERE acctid = $1", auth_schema));
    conn.prepare("UPDATE_LOGIN_LOGONPROOF",
                 fmt::format("UPDATE {}.accounts SET session_key_auth = decode($1, 'hex'), last_ip = $2, last_login = NOW(), locale = $3, os = $4, timezone_offset = $5 WHERE username = $6", auth_schema));
    conn.prepare("UPDATE_REALMLIST",
                 fmt::format("UPDATE {}.realmlist SET flag = $1, population = $2 WHERE id = $3", auth_schema));
    conn.prepare("INSERT_REALM_CHARACTERS",
                 fmt::format("INSERT INTO {}.realmcharacters (realmid, acctid, numchars) VALUES ($1, $2, $3)", auth_schema));
    conn.prepare("UPDATE_REALM_CHARACTERS",
                 fmt::format("UPDATE {}.realmcharacters SET numchars = $1 WHERE acctid = $2", auth_schema));
}

void DatabasePreparer::prepareRelaySchema(pqxx::connection &conn) {
    std::string relay_schema = getRelaySchema();

    conn.prepare("REPLACE_ACCOUNT_DATA",
                 fmt::format("INSERT INTO {}.account_data (account_id, type, time, data) "
                             "VALUES ($1, $2, $3, $4) "
                             "ON CONFLICT (account_id, type) DO UPDATE SET "
                             "time = EXCLUDED.time, data = EXCLUDED.data",
                             relay_schema));
    conn.prepare("SELECT_ACCOUNT_DATA",
                 fmt::format("SELECT account_id, type, time, data FROM {}.account_data WHERE account_id = $1", relay_schema));

    conn.prepare("SELECT_ADDONS",
                 fmt::format("SELECT name, crc FROM {}.addons", relay_schema));
    conn.prepare("SELECT_BANNED_ADDONS",
                 fmt::format("SELECT id, name, version, timestamp FROM {}.banned_addons ORDER BY timestamp", relay_schema));

    conn.prepare("REPLACE_CHARACTER_ACCOUNT_DATA",
                 fmt::format("INSERT INTO {}.character_account_data (guid, type, time, data) "
                             "VALUES ($1, $2, $3, $4) "
                             "ON CONFLICT (guid, type) DO UPDATE SET "
                             "time = EXCLUDED.time, data = EXCLUDED.data",
                             relay_schema));
    conn.prepare("SELECT_CHARACTER_ACCOUNT_DATA",
                 fmt::format("SELECT guid, type, time, data FROM {}.character_account_data WHERE guid = $1", relay_schema));

    conn.prepare("SELECT_ACCOUNT_TUTORIALS",
                 fmt::format("SELECT account_id, tut0, tut1, tut2, tut3, tut4, tut5, tut6, tut7 FROM {}.account_tutorial WHERE account_id = $1", relay_schema));

    conn.prepare("SELECT_CHAR_ENUM",
                 fmt::format("SELECT c.guid, c.name, c.race, c.class, c.gender, c.skin, c.face, c.hairStyle, c.hairColor, c.facialStyle, "
                             "c.level, c.zone, c.map, c.position_x, c.position_y, c.position_z, c.orientation, c.equipmentCache, c.playerFlags, c.at_login, c.istransfer, "
                             "gm.guildid as guild_guildid, "
                             "cp.entry as pet_entry, cp.modelid as pet_modelid, cp.level as pet_level, "
                             "cb.guid as banned_guid "
                             "FROM {}.characters AS c "
                             "LEFT JOIN {}.character_pet AS cp ON c.guid = cp.owner AND cp.slot = $1 "
                             "LEFT JOIN {}.guild_member AS gm ON c.guid = gm.guid "
                             "LEFT JOIN {}.character_banned AS cb ON c.guid = cb.guid AND cb.active = 1 "
                             "WHERE c.account = $2 AND c.deleteInfos_Name IS NULL "
                             "ORDER BY c.guid",
                             relay_schema, relay_schema, relay_schema, relay_schema));
    conn.prepare("SELECT_COUNT_CHARS_BY_USERNAME",
                 fmt::format("SELECT COUNT(*) "
                             "FROM {}.characters WHERE name = $1", relay_schema));

    conn.prepare("INSERT_CHARACTER",
            //                                         1       2    3      4      5       6     7    8     9     10      11         12          13          14         15          16
                 fmt::format("INSERT INTO {}.characters (account, name, race, class, gender, level, xp, money, skin, face, hairstyle, haircolor, facialstyle, bankslots, reststate, playerflags, "
                             //17       18              19              20          21          22          23         24      25        26       27        28
                             "map, instance_id, instance_mode_mask, position_x, position_y, position_z, orientation, trans_x, trans_y, trans_z, trans_o, transguid, "
                             // 29         30
                             "taximask, cinematic, "
                             //   31        32          33          34              35                 36                  37
                             "totaltime, leveltime, rest_bonus, logout_time, is_logout_resting, resettalents_cost, resettalents_time, "
                             //   38             39         40      41
                             "extra_flags, stable_slots, at_login, zone, "
                             //     42              43          44              45                46                47                 48
                             "death_expire_time, taxi_path, arenapoints, totalhonorpoints, todayhonorpoints, yesterdayhonorpoints, totalkills, "
                             //    49           50            51              52              53         54     55       56      57     58
                             "todaykills, yesterdaykills, chosentitle, knowncurrencies, watchedfaction, drunk, health, power1, power2, power3, "
                             // 59      60      61      62      63              64                65               66             67          68         69          70            71
                             "power4, power5, power6, power7, latency, talentgroupscount, activetalentgroup, exploredzones, equipmentcache, ammoid, knowntitles, actionbars, grantablelevels) "
                             "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $40, $41, $42, $43, $44, $45, $46, $47, $48, $49, $50, $51, $52, $53, $54, $55, $56, $57, $58, $59, $60, $61, $62, $63, $64, $65, $66, $67, $68, $69, $70, $71)",
                             relay_schema));

    conn.prepare("SELECT_PLAYER_CREATE_INFO",
                 fmt::format("SELECT race, class, map, zone, position_x, position_y, position_z, orientation FROM {}.playercreateinfo", relay_schema));
    conn.prepare("SELECT_PLAYER_CREATE_INFO_ITEM",
                 fmt::format("SELECT race, class, itemid, amount FROM {}.playercreateinfo_item", relay_schema));
    conn.prepare("SELECT_PLAYER_CREATE_INFO_SKILLS",
                 fmt::format("SELECT racemask, classmask, skill, rank, comment FROM {}.playercreateinfo_skills", relay_schema));

}

void DatabasePreparer::prepareDBCSchema(pqxx::connection &conn) {
    std::string dbc_schema = getDBCSchema();

    conn.prepare("SELECT_DBC_ACHIEVEMENT",
                 fmt::format(
                         "SELECT "
                         "id, faction, instance_id, supercedes, "
                         "title_lang_enus, title_lang_engb, title_lang_kokr, title_lang_frfr, title_lang_dede, "
                         "title_lang_encn, title_lang_zhcn, title_lang_entw, title_lang_zhtw, "
                         "title_lang_eses, title_lang_esmx, title_lang_ruru, title_lang_ptpt, "
                         "title_lang_ptbr, title_lang_itit, title_lang_unk, title_lang_mask, "
                         "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, description_lang_dede, "
                         "description_lang_encn, description_lang_zhcn, description_lang_entw, description_lang_zhtw, "
                         "description_lang_eses, description_lang_esmx, description_lang_ruru, description_lang_ptpt, "
                         "description_lang_ptbr, description_lang_itit, description_lang_unk, description_lang_mask, "
                         "category, points, ui_order, flags, iconid, "
                         "reward_lang_enus, reward_lang_engb, reward_lang_kokr, reward_lang_frfr, reward_lang_dede, "
                         "reward_lang_encn, reward_lang_zhcn, reward_lang_entw, reward_lang_zhtw, "
                         "reward_lang_eses, reward_lang_esmx, reward_lang_ruru, reward_lang_ptpt, "
                         "reward_lang_ptbr, reward_lang_itit, reward_lang_unk, reward_lang_mask, "
                         "minimum_criteria, shares_criteria "
                         "FROM {}.dbc_achievement",
                         dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_ACHIEVEMENT_CRITERIA",
                 fmt::format(
                         "SELECT "
                         "id, achievement_id, \"type\", asset_id, quantity, start_event, start_asset, fail_event, fail_asset, "
                         "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, description_lang_dede, "
                         "description_lang_encn, description_lang_zhcn, description_lang_entw, description_lang_zhtw, "
                         "description_lang_eses, description_lang_esmx, description_lang_ruru, description_lang_ptpt, "
                         "description_lang_ptbr, description_lang_itit, description_lang_unk, description_lang_mask, "
                         "flags, timer_start_event, timer_asset_id, timer_time, ui_order "
                         "FROM {}.dbc_achievement_criteria",
                         dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_AREATABLE",
                 fmt::format(
                         "SELECT "
                         "id, continent_id, parent_area_id, "
                         "area_bit, flags, sound_provider_pref, "
                         "sound_provider_pref_underwater, ambience_id, zone_music, "
                         "intro_sound, exploration_level, area_name_lang_enus, "
                         "area_name_lang_engb, area_name_lang_kokr, area_name_lang_frfr, "
                         "area_name_lang_dede, area_name_lang_encn, area_name_lang_zhcn, "
                         "area_name_lang_entw, area_name_lang_zhtw, area_name_lang_eses, "
                         "area_name_lang_esmx, area_name_lang_ruru, area_name_lang_ptpt, "
                         "area_name_lang_ptbr, area_name_lang_itit, area_name_lang_unk, "
                         "area_name_lang_mask, faction_group_mask, liquid_type_id_1, "
                         "liquid_type_id_2, liquid_type_id_3, liquid_type_id_4, "
                         "min_elevation, ambient_multiplier, lightid "
                         "FROM {}.dbc_areatable",
                         dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_AREAGROUP",
                 fmt::format(
                         "SELECT id, areaid_1, areaid_2, areaid_3, areaid_4, areaid_5, areaid_6, nextareaid "
                         "FROM {}.dbc_areagroup",
                         dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_AREATRIGGER",
                 fmt::format(
                         "SELECT id, continent_id, x, y, z, radius, box_length, box_width, box_height, box_yaw "
                         "FROM {}.dbc_areatrigger",
                         dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_AUCTIONHOUSE",
                 fmt::format(
                         "SELECT id, faction_id, deposit_rate, consignment_rate, "
                         "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, "
                         "name_lang_dede, name_lang_encn, name_lang_zhcn, name_lang_entw, "
                         "name_lang_zhtw, name_lang_eses, name_lang_esmx, name_lang_ruru, "
                         "name_lang_ptpt, name_lang_ptbr, name_lang_itit, name_lang_unk, "
                         "name_lang_mask "
                         "FROM {}.dbc_auctionhouse",
                         dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_BANKBAGSLOTPRICES",
                 fmt::format("SELECT id, cost FROM {}.dbc_bankbagslotprices", dbc_schema)
    );
    conn.prepare(
            "SELECT_DBC_BANNEDADDONS",
            fmt::format(
                    "SELECT id, namemd5_1, namemd5_2, namemd5_3, namemd5_4, "
                    "versionmd5_1, versionmd5_2, versionmd5_3, versionmd5_4, lastmodified, flags "
                    "FROM {}.dbc_bannedaddons",
                    dbc_schema
            )
    );
    conn.prepare(
            "SELECT_DBC_BARBERSHOPSTYLE",
            fmt::format(
                    "SELECT id, type, "
                    "displayname_lang_enus, displayname_lang_engb, displayname_lang_kokr, displayname_lang_frfr, "
                    "displayname_lang_dede, displayname_lang_encn, displayname_lang_zhcn, displayname_lang_entw, "
                    "displayname_lang_zhtw, displayname_lang_eses, displayname_lang_esmx, displayname_lang_ruru, "
                    "displayname_lang_ptpt, displayname_lang_ptbr, displayname_lang_itit, displayname_lang_unk, "
                    "displayname_lang_mask, "
                    "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, "
                    "description_lang_dede, description_lang_encn, description_lang_zhcn, description_lang_entw, "
                    "description_lang_zhtw, description_lang_eses, description_lang_esmx, description_lang_ruru, "
                    "description_lang_ptpt, description_lang_ptbr, description_lang_itit, description_lang_unk, "
                    "description_lang_mask, "
                    "cost_modifier, race, sex, data "
                    "FROM {}.dbc_barbershopstyle",
                    dbc_schema
            )
    );
    conn.prepare(
            "SELECT_DBC_BATTLEMASTERLIST",
            fmt::format(
                    "SELECT id, mapid_1, mapid_2, mapid_3, mapid_4, mapid_5, mapid_6, mapid_7, mapid_8, "
                    "instance_type, groups_allowed, "
                    "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                    "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, name_lang_eses, "
                    "name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                    "name_lang_unk, name_lang_mask, "
                    "max_group_size, holiday_worldstate, minlevel, maxlevel "
                    "FROM {}.dbc_battlemasterlist",
                    dbc_schema
            )
    );
    conn.prepare(
            "SELECT_DBC_CHARACTERFACIALHAIRSTYLES",
            fmt::format(
                    "SELECT id, raceid, sexid, variationid, "
                    "geoset_1, geoset_2, geoset_3, geoset_4, geoset_5 "
                    "FROM {}.dbc_characterfacialhairstyles",
                    dbc_schema
            )
    );
    conn.prepare(
            "SELECT_DBC_CHARSECTIONS",
            fmt::format(
                    "SELECT id, raceid, sexid, basesection, "
                    "texturename_1, texturename_2, texturename_3, "
                    "flags, variationindex, colorindex "
                    "FROM {}.dbc_charsections",
                    dbc_schema
            )
    );
    conn.prepare("SELECT_DBC_CHARSTARTOUTFIT",
                 fmt::format("SELECT "
                             "id, raceid, classid, sexid, outfitid, "
                             "itemid_1, itemid_2, itemid_3, itemid_4, itemid_5, itemid_6, "
                             "itemid_7, itemid_8, itemid_9, itemid_10, itemid_11, itemid_12, "
                             "itemid_13, itemid_14, itemid_15, itemid_16, itemid_17, itemid_18, "
                             "itemid_19, itemid_20, itemid_21, itemid_22, itemid_23, itemid_24, "
                             "displayitemid_1, displayitemid_2, displayitemid_3, displayitemid_4, displayitemid_5, displayitemid_6, "
                             "displayitemid_7, displayitemid_8, displayitemid_9, displayitemid_10, displayitemid_11, displayitemid_12, "
                             "displayitemid_13, displayitemid_14, displayitemid_15, displayitemid_16, displayitemid_17, displayitemid_18, "
                             "displayitemid_19, displayitemid_20, displayitemid_21, displayitemid_22, displayitemid_23, displayitemid_24, "
                             "inventorytype_1, inventorytype_2, inventorytype_3, inventorytype_4, inventorytype_5, inventorytype_6, "
                             "inventorytype_7, inventorytype_8, inventorytype_9, inventorytype_10, inventorytype_11, inventorytype_12, "
                             "inventorytype_13, inventorytype_14, inventorytype_15, inventorytype_16, inventorytype_17, inventorytype_18, "
                             "inventorytype_19, inventorytype_20, inventorytype_21, inventorytype_22, inventorytype_23, inventorytype_24 "
                             "FROM {}.dbc_charstartoutfit",
                             dbc_schema));
    conn.prepare(
            "SELECT_DBC_CHARTITLES",
            fmt::format(
                    "SELECT "
                    "id, condition_id, "
                    "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                    "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                    "name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, "
                    "name_lang_ptbr, name_lang_itit, name_lang_unk, name_lang_mask, "
                    "name1_lang_enus, name1_lang_engb, name1_lang_kokr, name1_lang_frfr, name1_lang_dede, "
                    "name1_lang_encn, name1_lang_zhcn, name1_lang_entw, name1_lang_zhtw, "
                    "name1_lang_eses, name1_lang_esmx, name1_lang_ruru, name1_lang_ptpt, "
                    "name1_lang_ptbr, name1_lang_itit, name1_lang_unk, name1_lang_mask, "
                    "mask_id "
                    "FROM {}.dbc_chartitles",
                    dbc_schema
            )
    );
    conn.prepare(
            "SELECT_DBC_CHATCHANNELS",
            fmt::format(
                    "SELECT id, flags, factiongroup, "
                    "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                    "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, name_lang_eses, "
                    "name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                    "name_lang_unk, name_lang_mask, "
                    "shortcut_lang_enus, shortcut_lang_engb, shortcut_lang_kokr, shortcut_lang_frfr, "
                    "shortcut_lang_dede, shortcut_lang_encn, shortcut_lang_zhcn, shortcut_lang_entw, "
                    "shortcut_lang_zhtw, shortcut_lang_eses, shortcut_lang_esmx, shortcut_lang_ruru, "
                    "shortcut_lang_ptpt, shortcut_lang_ptbr, shortcut_lang_itit, shortcut_lang_unk, "
                    "shortcut_lang_mask "
                    "FROM {}.dbc_chatchannels",
                    dbc_schema
            )
    );
    conn.prepare("SELECT_DBC_CHRCLASSES",
                 fmt::format("SELECT "
                             "id, field01, displaypower, petnametoken, "
                             "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                             "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                             "name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                             "name_lang_unk, name_lang_mask, "
                             "name_female_lang_enus, name_female_lang_engb, name_female_lang_kokr, name_female_lang_frfr, name_female_lang_dede, "
                             "name_female_lang_encn, name_female_lang_zhcn, name_female_lang_entw, name_female_lang_zhtw, "
                             "name_female_lang_eses, name_female_lang_esmx, name_female_lang_ruru, name_female_lang_ptpt, name_female_lang_ptbr, name_female_lang_itit, "
                             "name_female_lang_unk, name_female_lang_mask, "
                             "name_male_lang_enus, name_male_lang_engb, name_male_lang_kokr, name_male_lang_frfr, name_male_lang_dede, "
                             "name_male_lang_encn, name_male_lang_zhcn, name_male_lang_entw, name_male_lang_zhtw, "
                             "name_male_lang_eses, name_male_lang_esmx, name_male_lang_ruru, name_male_lang_ptpt, name_male_lang_ptbr, name_male_lang_itit, "
                             "name_male_lang_unk, name_male_lang_mask, "
                             "filename, spellclassset, flags, cinematicsequenceid, required_expansion "
                             "FROM {}.dbc_chrclasses", dbc_schema));
    conn.prepare("SELECT_DBC_CHRRACES",
                 fmt::format("SELECT "
                             "id, flags, factionid, explorationsoundid, "
                             "maledisplayid, femaledisplayid, clientprefix, "
                             "baselanguage, creaturetype, ressicknessspellid, "
                             "splashsoundid, clientfilestring, cinematicsequenceid, "
                             "alliance, "
                             "name_lang_enus, name_lang_engb, name_lang_kokr, "
                             "name_lang_frfr, name_lang_dede, name_lang_encn, "
                             "name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                             "name_lang_eses, name_lang_esmx, name_lang_ruru, "
                             "name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                             "name_lang_unk, name_lang_mask, "
                             "name_female_lang_enus, name_female_lang_engb, "
                             "name_female_lang_kokr, name_female_lang_frfr, "
                             "name_female_lang_dede, name_female_lang_encn, "
                             "name_female_lang_zhcn, name_female_lang_entw, "
                             "name_female_lang_zhtw, name_female_lang_eses, "
                             "name_female_lang_esmx, name_female_lang_ruru, "
                             "name_female_lang_ptpt, name_female_lang_ptbr, "
                             "name_female_lang_itit, name_female_lang_unk, "
                             "name_female_lang_mask, "
                             "name_male_lang_enus, name_male_lang_engb, "
                             "name_male_lang_kokr, name_male_lang_frfr, "
                             "name_male_lang_dede, name_male_lang_encn, "
                             "name_male_lang_zhcn, name_male_lang_entw, "
                             "name_male_lang_zhtw, name_male_lang_eses, "
                             "name_male_lang_esmx, name_male_lang_ruru, "
                             "name_male_lang_ptpt, name_male_lang_ptbr, "
                             "name_male_lang_itit, name_male_lang_unk, "
                             "name_male_lang_mask, "
                             "facialhaircustomization_1, facialhaircustomization_2, "
                             "haircustomization, required_expansion "
                             "FROM {}.dbc_chrraces", dbc_schema));
    conn.prepare("SELECT_DBC_CINEMATICCAMERA",
                 fmt::format("SELECT "
                             "id, model, soundid, "
                             "originx, originy, originz, originfacing "
                             "FROM {}.dbc_cinematiccamera",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CINEMATICSEQUENCES",
                 fmt::format("SELECT id, soundid, "
                             "camera_1, camera_2, camera_3, camera_4, camera_5, camera_6, camera_7, camera_8 "
                             "FROM {}.dbc_cinematicsequences",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CREATUREDISPLAYINFO",
                 fmt::format("SELECT "
                             "id, modelid, soundid, extendeddisplayinfoid, "
                             "creaturemodelscale, creaturemodelalpha, "
                             "texturevariation_1, texturevariation_2, texturevariation_3, "
                             "portraittexturename, bloodlevel, bloodid, npcsoundid, "
                             "particlecolorid, creaturegeosetdata, objecteffectpackageid "
                             "FROM {}.dbc_creaturedisplayinfo",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CREATUREDISPLAYINFOEXTRA",
                 fmt::format("SELECT id, displayraceid, displaysexid, skinid, faceid, "
                             "hairstyleid, haircolorid, facialhairid, "
                             "npcitemdisplay_1, npcitemdisplay_2, npcitemdisplay_3, npcitemdisplay_4, npcitemdisplay_5, "
                             "npcitemdisplay_6, npcitemdisplay_7, npcitemdisplay_8, npcitemdisplay_9, npcitemdisplay_10, npcitemdisplay_11, "
                             "flags, bakename "
                             "FROM {}.dbc_creaturedisplayinfoextra",
                             dbc_schema));

    conn.prepare("SELECT_DBC_SKILLRACECLASSINFO",
                 fmt::format("SELECT "
                             "id, skillid, racemask, classmask, "
                             "flags, minlevel, skilltierid, skillcostindex "
                             "FROM {}.dbc_skillraceclassinfo",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CREATUREFAMILY",
                 fmt::format("SELECT "
                             "id, minscale, minscalelevel, maxscale, maxscalelevel, "
                             "skillline_1, skillline_2, petfoodmask, pettalenttype, categoryenumid, "
                             "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                             "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                             "name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, "
                             "name_lang_itit, name_lang_unk, "
                             "name_lang_mask, iconfile "
                             "FROM {}.dbc_creaturefamily",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CREATUREMODELDATA",
                 fmt::format("SELECT "
                             "id, flags, modelname, sizeclass, modelscale, "
                             "bloodid, footprinttextureid, footprinttexturelength, footprinttexturewidth, footprintparticlescale, "
                             "foleymaterialid, footstepshakesize, deaththudshakesize, soundid, "
                             "collisionwidth, collisionheight, mountheight, "
                             "geoboxminx, geoboxminy, geoboxminz, "
                             "geoboxmaxx, geoboxmaxy, geoboxmaxz, "
                             "worldeffectscale, attachedeffectscale, "
                             "missilecollisionradius, missilecollisionpush, missilecollisionraise "
                             "FROM {}.dbc_creaturemodeldata",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CREATURESPELDATA",
                 fmt::format("SELECT "
                             "id, "
                             "spells_1, spells_2, spells_3, spells_4, "
                             "availability_1, availability_2, availability_3, availability_4 "
                             "FROM {}.dbc_creaturespelldata",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CREATURETYPE",
                 fmt::format("SELECT id, name_lang_enus, name_lang_engb, name_lang_kokr, "
                             "name_lang_frfr, name_lang_dede, name_lang_encn, name_lang_zhcn, "
                             "name_lang_entw, name_lang_zhtw, name_lang_eses, name_lang_esmx, "
                             "name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                             "name_lang_unk, name_lang_mask, flags FROM {}.dbc_creaturetype",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CURRENCYCATEGORY",
                 fmt::format("SELECT "
                             "id, flags, "
                             "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                             "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                             "name_lang_eses, name_lang_esmx, name_lang_ruru, "
                             "name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                             "name_lang_unk, name_lang_mask "
                             "FROM {}.dbc_currencycategory",
                             dbc_schema));
    conn.prepare("SELECT_DBC_CURRENCYTYPES",
                 fmt::format("SELECT id, itemid, categoryid, bitindex "
                             "FROM {}.dbc_currencytypes",
                             dbc_schema));
    conn.prepare("SELECT_DBC_DESTRUCTIBLEMODELDATA",
                 fmt::format(
                         "SELECT id, "
                         "state0_impact_effect_doodad_set, state0_ambient_doodad_set, "
                         "state1_wmo, state1_destruction_doodad_set, state1_impact_effect_doodad_set, state1_ambient_doodad_set, "
                         "state2_wmo, state2_destruction_doodad_set, state2_impact_effect_doodad_set, state2_ambient_doodad_set, "
                         "state3_wmo, state3_init_doodad_set, state3_ambient_doodad_set, "
                         "eject_direction, repair_ground_fx, do_not_highlight, "
                         "heal_effect, heal_effect_speed "
                         "FROM {}.dbc_destructiblemodeldata",
                         dbc_schema));
    conn.prepare("SELECT_DBC_DUNGEONENCOUNTER",
                 fmt::format(
                         "SELECT id, mapid, difficulty, orderindex, bit, "
                         "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                         "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, name_lang_eses, "
                         "name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                         "name_lang_unk, name_lang_mask, spelliconid "
                         "FROM {}.dbc_dungeonencounter",
                         dbc_schema));
    conn.prepare("SELECT_DBC_DURABILITYCOSTS",
                 fmt::format(
                         "SELECT id, "
                         "weaponsubclasscost_1, weaponsubclasscost_2, weaponsubclasscost_3, weaponsubclasscost_4, "
                         "weaponsubclasscost_5, weaponsubclasscost_6, weaponsubclasscost_7, weaponsubclasscost_8, "
                         "weaponsubclasscost_9, weaponsubclasscost_10, weaponsubclasscost_11, weaponsubclasscost_12, "
                         "weaponsubclasscost_13, weaponsubclasscost_14, weaponsubclasscost_15, weaponsubclasscost_16, "
                         "weaponsubclasscost_17, weaponsubclasscost_18, weaponsubclasscost_19, weaponsubclasscost_20, "
                         "weaponsubclasscost_21, "
                         "armorsubclasscost_1, armorsubclasscost_2, armorsubclasscost_3, armorsubclasscost_4, "
                         "armorsubclasscost_5, armorsubclasscost_6, armorsubclasscost_7, armorsubclasscost_8 "
                         "FROM {}.dbc_durabilitycosts",
                         dbc_schema));
    conn.prepare("SELECT_DBC_DURABILITYQUALITY",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_durabilityquality",
                             dbc_schema));
    conn.prepare("SELECT_DBC_EMOTES",
                 fmt::format("SELECT id, emoteslashcommand, animid, emoteflags, "
                             "emotespecproc, emotespecprocparam, eventsoundid "
                             "FROM {}.dbc_emotes",
                             dbc_schema));
    conn.prepare("SELECT_DBC_EMOTESTEXT",
                 fmt::format("SELECT id, name, emoteid, "
                             "emotetext_1, emotetext_2, emotetext_3, emotetext_4, "
                             "emotetext_5, emotetext_6, emotetext_7, emotetext_8, "
                             "emotetext_9, emotetext_10, emotetext_11, emotetext_12, "
                             "emotetext_13, emotetext_14, emotetext_15, emotetext_16 "
                             "FROM {}.dbc_emotestext",
                             dbc_schema));
    conn.prepare("SELECT_DBC_EMOTESTEXTSOUND",
                 fmt::format("SELECT id, emotestextid, raceid, sexid, soundid "
                             "FROM {}.dbc_emotestextsound",
                             dbc_schema));
    conn.prepare("SELECT_DBC_FACTION",
                 fmt::format("SELECT "
                             "id, reputationindex, "
                             "reputationracemask_1, reputationracemask_2, reputationracemask_3, reputationracemask_4, "
                             "reputationclassmask_1, reputationclassmask_2, reputationclassmask_3, reputationclassmask_4, "
                             "reputationbase_1, reputationbase_2, reputationbase_3, reputationbase_4, "
                             "reputationflags_1, reputationflags_2, reputationflags_3, reputationflags_4, "
                             "parentfactionid, parentfactionmod_1, parentfactionmod_2, parentfactioncap_1, parentfactioncap_2, "
                             "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                             "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, name_lang_eses, name_lang_esmx, "
                             "name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, name_lang_unk, name_lang_mask, "
                             "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, "
                             "description_lang_dede, description_lang_encn, description_lang_zhcn, description_lang_entw, "
                             "description_lang_zhtw, description_lang_eses, description_lang_esmx, description_lang_ruru, "
                             "description_lang_ptpt, description_lang_ptbr, description_lang_itit, description_lang_unk, "
                             "description_lang_mask "
                             "FROM {}.dbc_faction",
                             dbc_schema));
    conn.prepare("SELECT_DBC_FACTIONTEMPLATE",
                 fmt::format("SELECT id, faction, flags, factiongroup, friendgroup, enemygroup, "
                             "enemies_1, enemies_2, enemies_3, enemies_4, "
                             "friend_1, friend_2, friend_3, friend_4 "
                             "FROM {}.dbc_factiontemplate",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GAMEOBJECTARTKIT",
                 fmt::format("SELECT id, texturevariation_1, texturevariation_2, texturevariation_3, "
                             "attachmodel_1, attachmodel_2, attachmodel_3, attachmodel_4 "
                             "FROM {}.dbc_gameobjectartkit",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GAMEOBJECTDISPLAYINFO",
                 fmt::format(
                         "SELECT id, modelname, "
                         "sound_1, sound_2, sound_3, sound_4, sound_5, "
                         "sound_6, sound_7, sound_8, sound_9, sound_10, "
                         "geoboxminx, geoboxminy, geoboxminz, geoboxmaxx, geoboxmaxy, geoboxmaxz, "
                         "objecteffectpackageid "
                         "FROM {}.dbc_gameobjectdisplayinfo",
                         dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GEMPROPERTIES",
                 fmt::format("SELECT id, enchant_id, maxcount_inv, maxcount_item, type "
                             "FROM {}.dbc_gemproperties",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GLYPHPROPERTIES",
                 fmt::format("SELECT id, spellid, glyphslotflags, spelliconid "
                             "FROM {}.dbc_glyphproperties",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GLYPHSLOT",
                 fmt::format("SELECT id, type, tooltip "
                             "FROM {}.dbc_glyphslot",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GTBARBERSHOPCOSTBASE",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtbarbershopcostbase",
                             dbc_schema)
    );
    conn.prepare("SELECT_DBC_GTCHANGETOMELEECRIT",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtchancetomeleecrit",
                             dbc_schema)
    );
    conn.prepare("SELECT_DBC_GTCHANGETOMELEECRITBASE",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtchancetomeleecritbase",
                             dbc_schema));
    conn.prepare("SELECT_DBC_GTCHANGETOSPELLCRIT",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtchancetospellcrit",
                             dbc_schema));
    conn.prepare("SELECT_DBC_GTCHANGETOSPELLCRITBASE",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtchancetospellcritbase",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GTCOMBATRATINGS",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtcombatratings",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GTNPCMANACOSTSCALER",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtnpcmanacostscaler",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GTOCTCLASSCOMBATRATINGSALAR",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtoctclasscombatratingscalar",
                             dbc_schema
                 )
    );
    conn.prepare("SELECT_DBC_GTOCTREGENHP",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtoctregenhp",
                             dbc_schema)
    );
    conn.prepare("SELECT_DBC_GTOCTREGENMP",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtoctregenmp",
                             dbc_schema)
    );
    conn.prepare("SELECT_DBC_GTREGENHPPERSPT",
                 fmt::format("SELECT id, data "
                             "FROM {}.dbc_gtregenhpperspt",
                             dbc_schema));
    conn.prepare("SELECT_DBC_GTREGENMPPERSPT",
                 fmt::format("SELECT id, data FROM {}.dbc_gtregenmpperspt", dbc_schema)
    );
    conn.prepare("SELECT_DBC_HOLIDAYS",
                 fmt::format("SELECT id, duration_1, duration_2, duration_3, duration_4, duration_5, duration_6, duration_7, duration_8, duration_9, duration_10, "
                             "date_1, date_2, date_3, date_4, date_5, date_6, date_7, date_8, date_9, date_10, date_11, date_12, date_13, date_14, date_15, date_16, date_17, date_18, date_19, date_20, date_21, date_22, date_23, date_24, date_25, date_26, "
                             "region, looping, calendarflags_1, calendarflags_2, calendarflags_3, calendarflags_4, calendarflags_5, calendarflags_6, calendarflags_7, calendarflags_8, calendarflags_9, calendarflags_10, "
                             "holidaynameid, holidaydescriptionid, texturefilename, priority, calendarfiltertype, flags "
                             "FROM {}.dbc_holidays", dbc_schema));
    conn.prepare("SELECT_DBC_ITEM",
                 fmt::format("SELECT id, class_id, subclass_id, sound_override_subclassid, "
                             "material, display_info_id, inventory_type, sheathe_type "
                             "FROM {}.dbc_item",
                             dbc_schema));
    conn.prepare("SELECT_DBC_ITEMBAGFAMILY",
                 fmt::format("SELECT id, name_lang_enUS, name_lang_enGB, name_lang_koKR, name_lang_frFR, "
                             "name_lang_deDE, name_lang_enCN, name_lang_zhCN, name_lang_enTW, name_lang_zhTW, "
                             "name_lang_esES, name_lang_esMX, name_lang_ruRU, name_lang_ptPT, name_lang_ptBR, "
                             "name_lang_itIT, name_lang_Unk, name_lang_mask "
                             "FROM {}.dbc_itembagfamily",
                             dbc_schema));
    conn.prepare("SELECT_DBC_ITEMDISPLAYINFO",
                 fmt::format("SELECT id, model_name_1, model_name_2, "
                             "model_texture_1, model_texture_2, "
                             "inventory_icon_1, inventory_icon_2, "
                             "geoset_group_1, geoset_group_2, geoset_group_3, "
                             "flags, spell_visual_id, group_sound_index, "
                             "helmet_geoset_vis_1, helmet_geoset_vis_2, "
                             "texture_1, texture_2, texture_3, texture_4, "
                             "texture_5, texture_6, texture_7, texture_8, "
                             "item_visual, particle_color_id "
                             "FROM {}.dbc_itemdisplayinfo",
                             dbc_schema));
    conn.prepare("SELECT_DBC_ITEMEXTENDEDCOST",
                 fmt::format("SELECT "
                             "id, honor_points, arena_points, arena_bracket, "
                             "item_id_1, item_id_2, item_id_3, item_id_4, item_id_5, "
                             "item_count_1, item_count_2, item_count_3, item_count_4, item_count_5, "
                             "required_arena_rating, item_purchase_group "
                             "FROM {}.dbc_itemextendedcost",
                             dbc_schema));
    conn.prepare("SELECT_DBC_ITEMLIMITCATEGORY",
                 fmt::format("SELECT "
                             "id, name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, "
                             "name_lang_dede, name_lang_encn, name_lang_zhcn, name_lang_entw, "
                             "name_lang_zhtw, name_lang_eses, name_lang_esmx, name_lang_ruru, "
                             "name_lang_ptpt, name_lang_ptbr, name_lang_itit, name_lang_unk, "
                             "name_lang_mask, quantity, flags "
                             "FROM {}.dbc_itemlimitcategory",
                             dbc_schema));
    conn.prepare("SELECT_DBC_ITEMRANDOMPROPERTIES",
                 fmt::format(
                         "SELECT id, name, enchantment_1, enchantment_2, enchantment_3, enchantment_4, enchantment_5, "
                         "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                         "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                         "name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, "
                         "name_lang_itit, name_lang_unk, name_lang_mask "
                         "FROM {}.dbc_itemrandomproperties",
                         dbc_schema));
    conn.prepare("SELECT_DBC_ITEMRANDOMSUFFIX",
                 fmt::format("SELECT "
                             "id, "
                             "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                             "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                             "name_lang_eses, name_lang_esmx, name_lang_ruru, "
                             "name_lang_ptpt, name_lang_ptbr, name_lang_itit, name_lang_unk, "
                             "name_lang_mask, internalname, "
                             "enchantment_1, enchantment_2, enchantment_3, enchantment_4, enchantment_5, "
                             "allocationpct_1, allocationpct_2, allocationpct_3, allocationpct_4, allocationpct_5 "
                             "FROM {}.dbc_itemrandomsuffix",
                             dbc_schema));
    conn.prepare("SELECT_DBC_ITEMSET",
                 fmt::format("SELECT "
                             "id, "
                             "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                             "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                             "name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, "
                             "name_lang_ptbr, name_lang_itit, name_lang_unk, name_lang_mask, "
                             "itemid_1, itemid_2, itemid_3, itemid_4, itemid_5, itemid_6, itemid_7, "
                             "itemid_8, itemid_9, itemid_10, itemid_11, itemid_12, itemid_13, "
                             "itemid_14, itemid_15, itemid_16, itemid_17, "
                             "setspellid_1, setspellid_2, setspellid_3, setspellid_4, "
                             "setspellid_5, setspellid_6, setspellid_7, setspellid_8, "
                             "setthreshold_1, setthreshold_2, setthreshold_3, setthreshold_4, "
                             "setthreshold_5, setthreshold_6, setthreshold_7, setthreshold_8, "
                             "requiredskill, requiredskillrank "
                             "FROM {}.dbc_itemset", dbc_schema));
    conn.prepare("SELECT_DBC_LFGDUNGEONS",
                 fmt::format("SELECT "
                             "id, name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                             "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, name_lang_eses, "
                             "name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, name_lang_unk, name_lang_mask, "
                             "min_level, max_level, target_level, target_level_min, target_level_max, mapid, difficulty, flags, typeid, faction, "
                             "texturefilename, expansionlevel, order_index, group_id, "
                             "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, description_lang_dede, "
                             "description_lang_encn, description_lang_zhcn, description_lang_entw, description_lang_zhtw, description_lang_eses, "
                             "description_lang_esmx, description_lang_ruru, description_lang_ptpt, description_lang_ptbr, description_lang_itit, "
                             "description_lang_unk, description_lang_mask "
                             "FROM {}.dbc_lfgdungeons",
                             dbc_schema));
    conn.prepare("SELECT_DBC_LIGHT",
                 fmt::format("SELECT "
                             "id, continent_id, x, y, z, "
                             "falloff_start, falloff_end, "
                             "lightparamsid_1, lightparamsid_2, lightparamsid_3, lightparamsid_4, "
                             "lightparamsid_5, lightparamsid_6, lightparamsid_7, lightparamsid_8 "
                             "FROM {}.dbc_light",
                             dbc_schema));
    conn.prepare("SELECT_DBC_LIQUIDTYPE",
                 fmt::format(
                         "SELECT id, name, flags, type, soundid, spellid, "
                         "maxdarkendepth, fogdarkenintensity, ambdarkenintensity, dirdarkenintensity, "
                         "lightid, particlescale, particlemovement, particletexslots, materialid, "
                         "texture_1, texture_2, texture_3, texture_4, texture_5, texture_6, "
                         "color_1, color_2, "
                         "float_1, float_2, float_3, float_4, float_5, float_6, float_7, float_8, "
                         "float_9, float_10, float_11, float_12, float_13, float_14, float_15, float_16, float_17, float_18, "
                         "int_1, int_2, int_3, int_4 "
                         "FROM {}.dbc_liquidtype", dbc_schema));
    conn.prepare("SELECT_DBC_LOCK",
                 fmt::format("SELECT "
                             "id, type_1, type_2, type_3, type_4, type_5, type_6, type_7, type_8, "
                             "index_1, index_2, index_3, index_4, index_5, index_6, index_7, index_8, "
                             "skill_1, skill_2, skill_3, skill_4, skill_5, skill_6, skill_7, skill_8, "
                             "action_1, action_2, action_3, action_4, action_5, action_6, action_7, action_8 "
                             "FROM {}.dbc_lock", dbc_schema));
    conn.prepare("SELECT_DBC_MAILTEMPLATE",
                 fmt::format("SELECT id, "
                             "subject_lang_en_us, subject_lang_en_gb, subject_lang_ko_kr, subject_lang_fr_fr, "
                             "subject_lang_de_de, subject_lang_en_cn, subject_lang_zh_cn, subject_lang_en_tw, "
                             "subject_lang_zh_tw, subject_lang_es_es, subject_lang_es_mx, subject_lang_ru_ru, "
                             "subject_lang_pt_pt, subject_lang_pt_br, subject_lang_it_it, subject_lang_unk, subject_lang_mask, "
                             "body_lang_en_us, body_lang_en_gb, body_lang_ko_kr, body_lang_fr_fr, body_lang_de_de, "
                             "body_lang_en_cn, body_lang_zh_cn, body_lang_en_tw, body_lang_zh_tw, body_lang_es_es, "
                             "body_lang_es_mx, body_lang_ru_ru, body_lang_pt_pt, body_lang_pt_br, body_lang_it_it, "
                             "body_lang_unk, body_lang_mask "
                             "FROM {}.dbc_mailtemplate", dbc_schema));
    conn.prepare("SELECT_DBC_MAP",
                 fmt::format(
                         "SELECT id, directory, instance_type, flags, pvp, "
                         "mapname_lang_en_us, mapname_lang_en_gb, mapname_lang_ko_kr, mapname_lang_fr_fr, mapname_lang_de_de, "
                         "mapname_lang_en_cn, mapname_lang_zh_cn, mapname_lang_en_tw, mapname_lang_zh_tw, mapname_lang_es_es, "
                         "mapname_lang_es_mx, mapname_lang_ru_ru, mapname_lang_pt_pt, mapname_lang_pt_br, mapname_lang_it_it, "
                         "mapname_lang_unk, mapname_lang_mask, area_table_id, "
                         "mapdescription0_lang_en_us, mapdescription0_lang_en_gb, mapdescription0_lang_ko_kr, mapdescription0_lang_fr_fr, mapdescription0_lang_de_de, "
                         "mapdescription0_lang_en_cn, mapdescription0_lang_zh_cn, mapdescription0_lang_en_tw, mapdescription0_lang_zh_tw, mapdescription0_lang_es_es, "
                         "mapdescription0_lang_es_mx, mapdescription0_lang_ru_ru, mapdescription0_lang_pt_pt, mapdescription0_lang_pt_br, mapdescription0_lang_it_it, "
                         "mapdescription0_lang_unk, mapdescription0_lang_mask, "
                         "mapdescription1_lang_en_us, mapdescription1_lang_en_gb, mapdescription1_lang_ko_kr, mapdescription1_lang_fr_fr, mapdescription1_lang_de_de, "
                         "mapdescription1_lang_en_cn, mapdescription1_lang_zh_cn, mapdescription1_lang_en_tw, mapdescription1_lang_zh_tw, mapdescription1_lang_es_es, "
                         "mapdescription1_lang_es_mx, mapdescription1_lang_ru_ru, mapdescription1_lang_pt_pt, mapdescription1_lang_pt_br, mapdescription1_lang_it_it, "
                         "mapdescription1_lang_unk, mapdescription1_lang_mask, "
                         "loading_screen_id, minimap_icon_scale, corpse_map_id, corpse_x, corpse_y, time_of_day_override, expansion_id, raid_offset, max_players "
                         "FROM {}.dbc_map", dbc_schema));
    conn.prepare("SELECT_DBC_MAPDIFFICULTY",
                 fmt::format("SELECT id, map_id, difficulty, "
                             "message_lang_en_us, message_lang_en_gb, message_lang_ko_kr, message_lang_fr_fr, message_lang_de_de, "
                             "message_lang_en_cn, message_lang_zh_cn, message_lang_en_tw, message_lang_zh_tw, "
                             "message_lang_es_es, message_lang_es_mx, message_lang_ru_ru, message_lang_pt_pt, "
                             "message_lang_pt_br, message_lang_it_it, message_lang_unk, "
                             "message_lang_mask, raid_duration, max_players, difficulty_string "
                             "FROM {}.dbc_mapdifficulty", dbc_schema));
    conn.prepare("SELECT_DBC_MOVIE",
                 fmt::format(
                         "SELECT id, filename, volume "
                         "FROM {}.dbc_movie", dbc_schema));
    conn.prepare("SELECT_DBC_NAMESPROFANITY",
                 fmt::format("SELECT id, name, language "
                             "FROM {}.dbc_namesprofanity",
                             dbc_schema));
    conn.prepare("SELECT_DBC_NAMESRESERVED",
                 fmt::format("SELECT id, name, language "
                             "FROM {}.dbc_namesreserved",
                             dbc_schema));
    conn.prepare("SELECT_DBC_OVERRIDESPELLDATA",
                 fmt::format("SELECT id, spells_1, spells_2, spells_3, spells_4, spells_5, "
                             "spells_6, spells_7, spells_8, spells_9, spells_10, flags "
                             "FROM {}.dbc_overridespelldata", dbc_schema));
    conn.prepare("SELECT_DBC_POWERDISPLAY",
                 fmt::format("SELECT id, actualtype, globalstring_basetag, red, green, blue "
                             "FROM {}.dbc_powerdisplay", dbc_schema));
    conn.prepare("SELECT_DBC_PVPDifficulty",
                 fmt::format("SELECT id, map_id, range_index, min_level, max_level, difficulty "
                             "FROM {}.dbc_pvpdifficulty",
                             dbc_schema));
    conn.prepare("SELECT_DBC_QUESTFACTIONREWARD",
                 fmt::format(
                         "SELECT id, difficulty_1, difficulty_2, difficulty_3, difficulty_4, "
                         "difficulty_5, difficulty_6, difficulty_7, difficulty_8, difficulty_9, difficulty_10 "
                         "FROM {}.dbc_questfactionreward",
                         dbc_schema));
    conn.prepare("SELECT_DBC_QUESTSORT",
                 fmt::format(
                         "SELECT id, sortname_lang_enus, sortname_lang_engb, sortname_lang_kokr, sortname_lang_frfr, "
                         "sortname_lang_dede, sortname_lang_encn, sortname_lang_zhcn, sortname_lang_entw, "
                         "sortname_lang_zhtw, sortname_lang_eses, sortname_lang_esmx, sortname_lang_ruru, "
                         "sortname_lang_ptpt, sortname_lang_ptbr, sortname_lang_itit, sortname_lang_unk, sortname_lang_mask "
                         "FROM {}.dbc_questsort", dbc_schema));
    conn.prepare("SELECT_DBC_QUESTXP",
                 fmt::format(
                         "SELECT id, difficulty_1, difficulty_2, difficulty_3, difficulty_4, difficulty_5, "
                         "difficulty_6, difficulty_7, difficulty_8, difficulty_9, difficulty_10 "
                         "FROM {}.dbc_questxp",
                         dbc_schema));
    conn.prepare("SELECT_DBC_RANDPROPPOINTS",
                 fmt::format("SELECT id, epic_1, epic_2, epic_3, epic_4, epic_5, "
                             "superior_1, superior_2, superior_3, superior_4, superior_5, "
                             "good_1, good_2, good_3, good_4, good_5 "
                             "FROM {}.dbc_randproppoints", dbc_schema));
    conn.prepare("SELECT_DBC_SCALINGSTATDISTRIBUTION",
                 fmt::format("SELECT id, statid_1, statid_2, statid_3, statid_4, statid_5, "
                             "statid_6, statid_7, statid_8, statid_9, statid_10, "
                             "bonus_1, bonus_2, bonus_3, bonus_4, bonus_5, bonus_6, bonus_7, bonus_8, bonus_9, bonus_10, "
                             "maxlevel "
                             "FROM {}.dbc_scalingstatdistribution",
                             dbc_schema));
    conn.prepare("SELECT_DBC_SCALINGSTATVALUES",
                 fmt::format("SELECT id, charlevel, shoulderbudget, trinketbudget, weaponbudget1h, rangedbudget, "
                             "clothshoulderarmor, leathershoulderarmor, mailshoulderarmor, plateshoulderarmor, "
                             "weapondps1h, weapondps2h, spellcasterdps1h, spellcasterdps2h, "
                             "rangeddps, wanddps, spellpower, primarybudget, tertiarybudget, "
                             "clothcloakarmor, clothchestarmor, leatherchestarmor, mailchestarmor, platechestarmor "
                             "FROM {}.dbc_scalingstatvalues",
                             dbc_schema));
    conn.prepare("SELECT_DBC_SKILLLINE",
                 fmt::format("SELECT "
                             "id, categoryid, skillcostsid, "
                             "displayname_lang_enus, displayname_lang_engb, displayname_lang_kokr, displayname_lang_frfr, displayname_lang_dede, "
                             "displayname_lang_encn, displayname_lang_zhcn, displayname_lang_entw, displayname_lang_zhtw, "
                             "displayname_lang_eses, displayname_lang_esmx, displayname_lang_ruru, displayname_lang_ptpt, "
                             "displayname_lang_ptbr, displayname_lang_itit, displayname_lang_unk, displayname_lang_mask, "
                             "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, description_lang_dede, "
                             "description_lang_encn, description_lang_zhcn, description_lang_entw, description_lang_zhtw, "
                             "description_lang_eses, description_lang_esmx, description_lang_ruru, description_lang_ptpt, "
                             "description_lang_ptbr, description_lang_itit, description_lang_unk, description_lang_mask, "
                             "spelliconid, "
                             "alternateverb_lang_enus, alternateverb_lang_engb, alternateverb_lang_kokr, alternateverb_lang_frfr, alternateverb_lang_dede, "
                             "alternateverb_lang_encn, alternateverb_lang_zhcn, alternateverb_lang_entw, alternateverb_lang_zhtw, "
                             "alternateverb_lang_eses, alternateverb_lang_esmx, alternateverb_lang_ruru, alternateverb_lang_ptpt, "
                             "alternateverb_lang_ptbr, alternateverb_lang_itit, alternateverb_lang_unk, alternateverb_lang_mask, "
                             "canlink "
                             "FROM {}.dbc_skillline",
                             dbc_schema));
    conn.prepare("SELECT_DBC_SKILLLINEABILITY",
                 fmt::format("SELECT id, skillline, spell, racemask, classmask, "
                             "excluderace, excludeclass, minskilllinerank, supercededbyspell, "
                             "acquiremethod, trivialskilllinerankhigh, trivialskilllineranklow, "
                             "tradeskillcategoryid "
                             "FROM {}.dbc_skilllineability", dbc_schema));
    conn.prepare("SELECT_DBC_SKILLTIERS",
                 fmt::format(
                         "SELECT id, "
                         "cost_1, cost_2, cost_3, cost_4, cost_5, cost_6, cost_7, cost_8, cost_9, cost_10, "
                         "cost_11, cost_12, cost_13, cost_14, cost_15, cost_16, "
                         "value_1, value_2, value_3, value_4, value_5, value_6, value_7, value_8, value_9, value_10, "
                         "value_11, value_12, value_13, value_14, value_15, value_16 "
                         "FROM {}.dbc_skilltiers",
                         dbc_schema
                 )
    );

}

void DatabasePreparer::prepareWorldSchema(pqxx::connection &conn) {
    std::string world_schema = getWorldSchema();

    conn.prepare("SELECT_ITEM_TEMPLATE",
                 fmt::format("SELECT "
                             "entry, class, subclass, soundoverridesubclass, "
                             "name, displayid, quality, flags, flagsextra, "
                             "buycount, buyprice, sellprice, inventorytype, "
                             "allowableclass, allowablerace, itemlevel, "
                             "requiredlevel, requiredskill, requiredskillrank, "
                             "requiredspell, requiredhonorrank, requiredcityrank, "
                             "requiredreputationfaction, requiredreputationrank, "
                             "maxcount, stackable, containerslots, statscount, "
                             "stat_type1, stat_value1, stat_type2, stat_value2, "
                             "stat_type3, stat_value3, stat_type4, stat_value4, "
                             "stat_type5, stat_value5, stat_type6, stat_value6, "
                             "stat_type7, stat_value7, stat_type8, stat_value8, "
                             "stat_type9, stat_value9, stat_type10, stat_value10, "
                             "scalingstatdistribution, scalingstatvalue, "
                             "dmg_min1, dmg_max1, dmg_type1, dmg_min2, dmg_max2, dmg_type2, "
                             "armor, holy_res, fire_res, nature_res, frost_res, shadow_res, arcane_res, "
                             "delay, ammo_type, rangedmodrange, "
                             "spellid_1, spelltrigger_1, spellcharges_1, spellppmrate_1, spellcooldown_1, spellcategory_1, spellcategorycooldown_1, "
                             "spellid_2, spelltrigger_2, spellcharges_2, spellppmrate_2, spellcooldown_2, spellcategory_2, spellcategorycooldown_2, "
                             "spellid_3, spelltrigger_3, spellcharges_3, spellppmrate_3, spellcooldown_3, spellcategory_3, spellcategorycooldown_3, "
                             "spellid_4, spelltrigger_4, spellcharges_4, spellppmrate_4, spellcooldown_4, spellcategory_4, spellcategorycooldown_4, "
                             "spellid_5, spelltrigger_5, spellcharges_5, spellppmrate_5, spellcooldown_5, spellcategory_5, spellcategorycooldown_5, "
                             "bonding, description, pagetext, languageid, pagematerial, "
                             "startquest, lockid, material, sheath, "
                             "randomproperty, randomsuffix, block, itemset, maxdurability, "
                             "area, map, bagfamily, totemcategory, "
                             "socketcolor_1, socketcontent_1, socketcolor_2, socketcontent_2, socketcolor_3, socketcontent_3, "
                             "socketbonus, gemproperties, requireddisenchantskill, armordamagemodifier, "
                             "duration, itemlimitcategory, holidayid, scriptname, "
                             "disenchantid, foodtype, minmoneyloot, maxmoneyloot, flagscustom, verifiedbuild "
                             "FROM {}.item_template", world_schema));
}