#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_mailtemplate **/
struct PgDbcMailTemplate {
    uint32_t id;

    // Subject fields
    std::optional<std::string> subject_lang_en_us;
    std::optional<std::string> subject_lang_en_gb;
    std::optional<std::string> subject_lang_ko_kr;
    std::optional<std::string> subject_lang_fr_fr;
    std::optional<std::string> subject_lang_de_de;
    std::optional<std::string> subject_lang_en_cn;
    std::optional<std::string> subject_lang_zh_cn;
    std::optional<std::string> subject_lang_en_tw;
    std::optional<std::string> subject_lang_zh_tw;
    std::optional<std::string> subject_lang_es_es;
    std::optional<std::string> subject_lang_es_mx;
    std::optional<std::string> subject_lang_ru_ru;
    std::optional<std::string> subject_lang_pt_pt;
    std::optional<std::string> subject_lang_pt_br;
    std::optional<std::string> subject_lang_it_it;
    std::optional<std::string> subject_lang_unk;
    uint32_t subject_lang_mask;

    // Body fields
    std::optional<std::string> body_lang_en_us;
    std::optional<std::string> body_lang_en_gb;
    std::optional<std::string> body_lang_ko_kr;
    std::optional<std::string> body_lang_fr_fr;
    std::optional<std::string> body_lang_de_de;
    std::optional<std::string> body_lang_en_cn;
    std::optional<std::string> body_lang_zh_cn;
    std::optional<std::string> body_lang_en_tw;
    std::optional<std::string> body_lang_zh_tw;
    std::optional<std::string> body_lang_es_es;
    std::optional<std::string> body_lang_es_mx;
    std::optional<std::string> body_lang_ru_ru;
    std::optional<std::string> body_lang_pt_pt;
    std::optional<std::string> body_lang_pt_br;
    std::optional<std::string> body_lang_it_it;
    std::optional<std::string> body_lang_unk;
    uint32_t body_lang_mask;
};

template<>
struct PgRowMapper<PgDbcMailTemplate> {
    static PgDbcMailTemplate map(const pqxx::row& r) {
        PgDbcMailTemplate m{};
        m.id = r["id"].as<uint32_t>();

        // Subject fields
        m.subject_lang_en_us = get_optional_string(r, "subject_lang_en_us");
        m.subject_lang_en_gb = get_optional_string(r, "subject_lang_en_gb");
        m.subject_lang_ko_kr = get_optional_string(r, "subject_lang_ko_kr");
        m.subject_lang_fr_fr = get_optional_string(r, "subject_lang_fr_fr");
        m.subject_lang_de_de = get_optional_string(r, "subject_lang_de_de");
        m.subject_lang_en_cn = get_optional_string(r, "subject_lang_en_cn");
        m.subject_lang_zh_cn = get_optional_string(r, "subject_lang_zh_cn");
        m.subject_lang_en_tw = get_optional_string(r, "subject_lang_en_tw");
        m.subject_lang_zh_tw = get_optional_string(r, "subject_lang_zh_tw");
        m.subject_lang_es_es = get_optional_string(r, "subject_lang_es_es");
        m.subject_lang_es_mx = get_optional_string(r, "subject_lang_es_mx");
        m.subject_lang_ru_ru = get_optional_string(r, "subject_lang_ru_ru");
        m.subject_lang_pt_pt = get_optional_string(r, "subject_lang_pt_pt");
        m.subject_lang_pt_br = get_optional_string(r, "subject_lang_pt_br");
        m.subject_lang_it_it = get_optional_string(r, "subject_lang_it_it");
        m.subject_lang_unk   = get_optional_string(r, "subject_lang_unk");
        m.subject_lang_mask  = r["subject_lang_mask"].as<uint32_t>();

        // Body fields
        m.body_lang_en_us = get_optional_string(r, "body_lang_en_us");
        m.body_lang_en_gb = get_optional_string(r, "body_lang_en_gb");
        m.body_lang_ko_kr = get_optional_string(r, "body_lang_ko_kr");
        m.body_lang_fr_fr = get_optional_string(r, "body_lang_fr_fr");
        m.body_lang_de_de = get_optional_string(r, "body_lang_de_de");
        m.body_lang_en_cn = get_optional_string(r, "body_lang_en_cn");
        m.body_lang_zh_cn = get_optional_string(r, "body_lang_zh_cn");
        m.body_lang_en_tw = get_optional_string(r, "body_lang_en_tw");
        m.body_lang_zh_tw = get_optional_string(r, "body_lang_zh_tw");
        m.body_lang_es_es = get_optional_string(r, "body_lang_es_es");
        m.body_lang_es_mx = get_optional_string(r, "body_lang_es_mx");
        m.body_lang_ru_ru = get_optional_string(r, "body_lang_ru_ru");
        m.body_lang_pt_pt = get_optional_string(r, "body_lang_pt_pt");
        m.body_lang_pt_br = get_optional_string(r, "body_lang_pt_br");
        m.body_lang_it_it = get_optional_string(r, "body_lang_it_it");
        m.body_lang_unk   = get_optional_string(r, "body_lang_unk");
        m.body_lang_mask  = r["body_lang_mask"].as<uint32_t>();

        return m;
    }
};