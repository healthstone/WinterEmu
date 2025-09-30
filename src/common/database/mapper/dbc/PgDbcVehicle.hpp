#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcVehicle {
    uint32_t id;
    int32_t flags;
    float turnspeed;
    float pitchspeed;
    float pitchmin;
    float pitchmax;

    uint32_t seatid_1;
    uint32_t seatid_2;
    uint32_t seatid_3;
    uint32_t seatid_4;
    uint32_t seatid_5;
    uint32_t seatid_6;
    uint32_t seatid_7;
    uint32_t seatid_8;

    float mouselookoffsetpitch;
    float camerafadedistscalarmin;
    float camerafadedistscalarmax;
    float camerapitchoffset;
    float facinglimitright;
    float facinglimitleft;
    float mssltrgtturnlingering;
    float mssltrgtpitchlingering;
    float mssltrgtmouselingering;
    float mssltrgtendopacity;
    float mssltrgtarcspeed;
    float mssltrgtarcrepeat;
    float mssltrgtarcwidth;
    float mssltrgtimpactradius_1;
    float mssltrgtimpactradius_2;

    std::optional<std::string> mssltrgtarctexture;
    std::optional<std::string> mssltrgtimpacttexture;
    std::optional<std::string> mssltrgtimpactmodel_1;
    std::optional<std::string> mssltrgtimpactmodel_2;

    float camerayawoffset;
    uint32_t uilocomotiontype;
    float mssltrgtimpacttexradius;
    uint32_t vehicleuiindicatorid;
    int32_t powerdisplayid_1;
    int32_t powerdisplayid_2;
    int32_t powerdisplayid_3;
};

template <>
struct PgRowMapper<DbcVehicle> {
    static DbcVehicle map(const pqxx::row& r) {
        DbcVehicle row{};
        row.id = r["id"].as<uint32_t>();
        row.flags = r["flags"].as<int32_t>();
        row.turnspeed = r["turnspeed"].as<float>();
        row.pitchspeed = r["pitchspeed"].as<float>();
        row.pitchmin = r["pitchmin"].as<float>();
        row.pitchmax = r["pitchmax"].as<float>();

        row.seatid_1 = r["seatid_1"].as<uint32_t>();
        row.seatid_2 = r["seatid_2"].as<uint32_t>();
        row.seatid_3 = r["seatid_3"].as<uint32_t>();
        row.seatid_4 = r["seatid_4"].as<uint32_t>();
        row.seatid_5 = r["seatid_5"].as<uint32_t>();
        row.seatid_6 = r["seatid_6"].as<uint32_t>();
        row.seatid_7 = r["seatid_7"].as<uint32_t>();
        row.seatid_8 = r["seatid_8"].as<uint32_t>();

        row.mouselookoffsetpitch = r["mouselookoffsetpitch"].as<float>();
        row.camerafadedistscalarmin = r["camerafadedistscalarmin"].as<float>();
        row.camerafadedistscalarmax = r["camerafadedistscalarmax"].as<float>();
        row.camerapitchoffset = r["camerapitchoffset"].as<float>();
        row.facinglimitright = r["facinglimitright"].as<float>();
        row.facinglimitleft = r["facinglimitleft"].as<float>();
        row.mssltrgtturnlingering = r["mssltrgtturnlingering"].as<float>();
        row.mssltrgtpitchlingering = r["mssltrgtpitchlingering"].as<float>();
        row.mssltrgtmouselingering = r["mssltrgtmouselingering"].as<float>();
        row.mssltrgtendopacity = r["mssltrgtendopacity"].as<float>();
        row.mssltrgtarcspeed = r["mssltrgtarcspeed"].as<float>();
        row.mssltrgtarcrepeat = r["mssltrgtarcrepeat"].as<float>();
        row.mssltrgtarcwidth = r["mssltrgtarcwidth"].as<float>();
        row.mssltrgtimpactradius_1 = r["mssltrgtimpactradius_1"].as<float>();
        row.mssltrgtimpactradius_2 = r["mssltrgtimpactradius_2"].as<float>();

        row.mssltrgtarctexture = get_optional_string(r, "mssltrgtarctexture");
        row.mssltrgtimpacttexture = get_optional_string(r, "mssltrgtimpacttexture");
        row.mssltrgtimpactmodel_1 = get_optional_string(r, "mssltrgtimpactmodel_1");
        row.mssltrgtimpactmodel_2 = get_optional_string(r, "mssltrgtimpactmodel_2");

        row.camerayawoffset = r["camerayawoffset"].as<float>();
        row.uilocomotiontype = r["uilocomotiontype"].as<uint32_t>();
        row.mssltrgtimpacttexradius = r["mssltrgtimpacttexradius"].as<float>();
        row.vehicleuiindicatorid = r["vehicleuiindicatorid"].as<uint32_t>();
        row.powerdisplayid_1 = r["powerdisplayid_1"].as<int32_t>();
        row.powerdisplayid_2 = r["powerdisplayid_2"].as<int32_t>();
        row.powerdisplayid_3 = r["powerdisplayid_3"].as<int32_t>();
        return row;
    }
};