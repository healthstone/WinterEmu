#include "ObjectGuid.hpp"
#include "spdlog/fmt/bundled/format.h"

ObjectGuid const ObjectGuid::Empty = ObjectGuid();

std::string ObjectGuid::ToString() const {
    if (HasEntry()) {
        return fmt::format("GUID Full: 0x{:016X} Type: {} {} {} Low: {}",
                           _guid,
                           GetTypeName(),
                           (IsPet() ? "Pet number:" : "Entry:"),
                           GetEntry(),
                           GetCounter());
    } else {
        return fmt::format("GUID Full: 0x{:016X} Type: {} Low: {}",
                           _guid,
                           GetTypeName(),
                           GetCounter());
    }
}

std::string ObjectGuid::ToHexString() const {
    return fmt::format("0x{:016X}", _guid);
}

std::string_view ObjectGuid::GetTypeName(HighGuid high) {
    switch (high) {
        case HighGuid::Item:
            return "Item";
        case HighGuid::Player:
            return "Player";
        case HighGuid::GameObject:
            return "Gameobject";
        case HighGuid::Transport:
            return "Transport";
        case HighGuid::Unit:
            return "Creature";
        case HighGuid::Pet:
            return "Pet";
        case HighGuid::Vehicle:
            return "Vehicle";
        case HighGuid::DynamicObject:
            return "DynObject";
        case HighGuid::Corpse:
            return "Corpse";
        case HighGuid::Mo_Transport:
            return "MoTransport";
        case HighGuid::Instance:
            return "InstanceID";
        case HighGuid::Group:
            return "Group";
        default:
            return "<unknown>";
    }
}

ObjectGuid ObjectGuid::Global(HighGuid type, LowType counter) {
    return ObjectGuid(type, counter);
}

ObjectGuid ObjectGuid::MapSpecific(HighGuid type, uint32_t entry, LowType counter) {
    return ObjectGuid(type, entry, counter);
}