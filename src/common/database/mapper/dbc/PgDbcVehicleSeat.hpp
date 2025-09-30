#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcVehicleSeat {
    uint32_t id;
    int32_t flags;
    int32_t attachment_id;
    float attachment_offset_x;
    float attachment_offset_y;
    float attachment_offset_z;
    float enter_pre_delay;
    float enter_speed;
    float enter_gravity;
    float enter_min_duration;
    float enter_max_duration;
    float enter_min_arc_height;
    float enter_max_arc_height;
    int32_t enter_anim_start;
    int32_t enter_anim_loop;
    int32_t ride_anim_start;
    int32_t ride_anim_loop;
    int32_t ride_upper_anim_start;
    int32_t ride_upper_anim_loop;
    float exit_pre_delay;
    float exit_speed;
    float exit_gravity;
    float exit_min_duration;
    float exit_max_duration;
    float exit_min_arc_height;
    float exit_max_arc_height;
    int32_t exit_anim_start;
    int32_t exit_anim_loop;
    int32_t exit_anim_end;
    float passenger_yaw;
    float passenger_pitch;
    float passenger_roll;
    int32_t passenger_attachment_id;
    int32_t vehicle_enter_anim;
    int32_t vehicle_exit_anim;
    int32_t vehicle_ride_anim_loop;
    int32_t vehicle_enter_anim_bone;
    int32_t vehicle_exit_anim_bone;
    int32_t vehicle_ride_anim_loop_bone;
    float vehicle_enter_anim_delay;
    float vehicle_exit_anim_delay;
    uint32_t vehicle_ability_display;
    uint32_t enter_ui_sound_id;
    uint32_t exit_ui_sound_id;
    int32_t ui_skin;
    uint32_t flags_b;
    float camera_entering_delay;
    float camera_entering_duration;
    float camera_exiting_delay;
    float camera_exiting_duration;
    float camera_offset_x;
    float camera_offset_y;
    float camera_offset_z;
    float camera_pos_chase_rate;
    float camera_facing_chase_rate;
    float camera_entering_zoom;
    float camera_seat_zoom_min;
    float camera_seat_zoom_max;
};

template <>
struct PgRowMapper<DbcVehicleSeat> {
    static DbcVehicleSeat map(const pqxx::row& r) {
        DbcVehicleSeat row{};

        row.id = r["id"].as<uint32_t>();
        row.flags = r["flags"].as<int32_t>();
        row.attachment_id = r["attachment_id"].as<int32_t>();
        row.attachment_offset_x = r["attachment_offset_x"].as<float>();
        row.attachment_offset_y = r["attachment_offset_y"].as<float>();
        row.attachment_offset_z = r["attachment_offset_z"].as<float>();
        row.enter_pre_delay = r["enter_pre_delay"].as<float>();
        row.enter_speed = r["enter_speed"].as<float>();
        row.enter_gravity = r["enter_gravity"].as<float>();
        row.enter_min_duration = r["enter_min_duration"].as<float>();
        row.enter_max_duration = r["enter_max_duration"].as<float>();
        row.enter_min_arc_height = r["enter_min_arc_height"].as<float>();
        row.enter_max_arc_height = r["enter_max_arc_height"].as<float>();
        row.enter_anim_start = r["enter_anim_start"].as<int32_t>();
        row.enter_anim_loop = r["enter_anim_loop"].as<int32_t>();
        row.ride_anim_start = r["ride_anim_start"].as<int32_t>();
        row.ride_anim_loop = r["ride_anim_loop"].as<int32_t>();
        row.ride_upper_anim_start = r["ride_upper_anim_start"].as<int32_t>();
        row.ride_upper_anim_loop = r["ride_upper_anim_loop"].as<int32_t>();
        row.exit_pre_delay = r["exit_pre_delay"].as<float>();
        row.exit_speed = r["exit_speed"].as<float>();
        row.exit_gravity = r["exit_gravity"].as<float>();
        row.exit_min_duration = r["exit_min_duration"].as<float>();
        row.exit_max_duration = r["exit_max_duration"].as<float>();
        row.exit_min_arc_height = r["exit_min_arc_height"].as<float>();
        row.exit_max_arc_height = r["exit_max_arc_height"].as<float>();
        row.exit_anim_start = r["exit_anim_start"].as<int32_t>();
        row.exit_anim_loop = r["exit_anim_loop"].as<int32_t>();
        row.exit_anim_end = r["exit_anim_end"].as<int32_t>();
        row.passenger_yaw = r["passenger_yaw"].as<float>();
        row.passenger_pitch = r["passenger_pitch"].as<float>();
        row.passenger_roll = r["passenger_roll"].as<float>();
        row.passenger_attachment_id = r["passenger_attachment_id"].as<int32_t>();
        row.vehicle_enter_anim = r["vehicle_enter_anim"].as<int32_t>();
        row.vehicle_exit_anim = r["vehicle_exit_anim"].as<int32_t>();
        row.vehicle_ride_anim_loop = r["vehicle_ride_anim_loop"].as<int32_t>();
        row.vehicle_enter_anim_bone = r["vehicle_enter_anim_bone"].as<int32_t>();
        row.vehicle_exit_anim_bone = r["vehicle_exit_anim_bone"].as<int32_t>();
        row.vehicle_ride_anim_loop_bone = r["vehicle_ride_anim_loop_bone"].as<int32_t>();
        row.vehicle_enter_anim_delay = r["vehicle_enter_anim_delay"].as<float>();
        row.vehicle_exit_anim_delay = r["vehicle_exit_anim_delay"].as<float>();
        row.vehicle_ability_display = r["vehicle_ability_display"].as<uint32_t>();
        row.enter_ui_sound_id = r["enter_ui_sound_id"].as<uint32_t>();
        row.exit_ui_sound_id = r["exit_ui_sound_id"].as<uint32_t>();
        row.ui_skin = r["ui_skin"].as<int32_t>();
        row.flags_b = r["flags_b"].as<uint32_t>();
        row.camera_entering_delay = r["camera_entering_delay"].as<float>();
        row.camera_entering_duration = r["camera_entering_duration"].as<float>();
        row.camera_exiting_delay = r["camera_exiting_delay"].as<float>();
        row.camera_exiting_duration = r["camera_exiting_duration"].as<float>();
        row.camera_offset_x = r["camera_offset_x"].as<float>();
        row.camera_offset_y = r["camera_offset_y"].as<float>();
        row.camera_offset_z = r["camera_offset_z"].as<float>();
        row.camera_pos_chase_rate = r["camera_pos_chase_rate"].as<float>();
        row.camera_facing_chase_rate = r["camera_facing_chase_rate"].as<float>();
        row.camera_entering_zoom = r["camera_entering_zoom"].as<float>();
        row.camera_seat_zoom_min = r["camera_seat_zoom_min"].as<float>();
        row.camera_seat_zoom_max = r["camera_seat_zoom_max"].as<float>();

        return row;
    }
};