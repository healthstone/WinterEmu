#pragma once

#include <array>
#include <functional>
#include <list>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <cstdint>
#include <compare>

enum TypeID {
    TYPEID_OBJECT = 0,
    TYPEID_ITEM = 1,
    TYPEID_CONTAINER = 2,
    TYPEID_UNIT = 3,
    TYPEID_PLAYER = 4,
    TYPEID_GAMEOBJECT = 5,
    TYPEID_DYNAMICOBJECT = 6,
    TYPEID_CORPSE = 7
};

#define NUM_CLIENT_OBJECT_TYPES 8

enum TypeMask {
    TYPEMASK_OBJECT = 0x0001,
    TYPEMASK_ITEM = 0x0002,
    TYPEMASK_CONTAINER = 0x0004,
    TYPEMASK_UNIT = 0x0008,
    TYPEMASK_PLAYER = 0x0010,
    TYPEMASK_GAMEOBJECT = 0x0020,
    TYPEMASK_DYNAMICOBJECT = 0x0040,
    TYPEMASK_CORPSE = 0x0080,

    TYPEMASK_SEER = TYPEMASK_UNIT | TYPEMASK_PLAYER | TYPEMASK_DYNAMICOBJECT,
    TYPEMASK_WORLDOBJECT = TYPEMASK_UNIT | TYPEMASK_GAMEOBJECT | TYPEMASK_DYNAMICOBJECT | TYPEMASK_CORPSE
};

enum class HighGuid {
    Item = 0x4000,
    Container = 0x4000,
    Player = 0x0000,
    GameObject = 0xF110,
    Transport = 0xF120,
    Unit = 0xF130,
    Pet = 0xF140,
    Vehicle = 0xF150,
    DynamicObject = 0xF100,
    Corpse = 0xF101,
    Mo_Transport = 0x1FC0,
    Instance = 0x1F40,
    Group = 0x1F50,
};

template<HighGuid high>
struct ObjectGuidTraits {
    static bool const Global = false;
    static bool const MapSpecific = false;
};

#define GUID_TRAIT_GLOBAL(highguid) \
    template<> struct ObjectGuidTraits<highguid> \
    { \
        static bool const Global = true; \
        static bool const MapSpecific = false; \
    };

#define GUID_TRAIT_MAP_SPECIFIC(highguid) \
    template<> struct ObjectGuidTraits<highguid> \
    { \
        static bool const Global = false; \
        static bool const MapSpecific = true; \
    };

GUID_TRAIT_GLOBAL(HighGuid::Player)
GUID_TRAIT_GLOBAL(HighGuid::Item)
GUID_TRAIT_GLOBAL(HighGuid::Mo_Transport)
GUID_TRAIT_GLOBAL(HighGuid::Group)
GUID_TRAIT_GLOBAL(HighGuid::Instance)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Transport)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Unit)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Vehicle)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Pet)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::GameObject)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::DynamicObject)
GUID_TRAIT_MAP_SPECIFIC(HighGuid::Corpse)

class ObjectGuid {
public:
    static ObjectGuid const Empty;

    typedef uint32_t LowType;

    template<HighGuid type>
    static typename std::enable_if<ObjectGuidTraits<type>::Global, ObjectGuid>::type
    Create(LowType counter) { return Global(type, counter); }

    template<HighGuid type>
    static typename std::enable_if<ObjectGuidTraits<type>::MapSpecific, ObjectGuid>::type
    Create(uint32_t entry, LowType counter) { return MapSpecific(type, entry, counter); }

    ObjectGuid() : _guid(0) {}

    explicit ObjectGuid(uint64_t guid) : _guid(guid) {}

    ObjectGuid(HighGuid hi, uint32_t entry, LowType counter) : _guid(
            counter ? uint64_t(counter) | (uint64_t(entry) << 24) | (uint64_t(hi) << 48) : 0) {}

    ObjectGuid(HighGuid hi, LowType counter) : _guid(counter ? uint64_t(counter) | (uint64_t(hi) << 48) : 0) {}

    operator uint64_t() const { return _guid; }

    void Set(uint64_t guid) { _guid = guid; }

    void Clear() { _guid = 0; }

    uint64_t GetRawValue() const { return _guid; }

    HighGuid GetHigh() const { return HighGuid((_guid >> 48) & 0x0000FFFF); }

    uint32_t GetEntry() const { return HasEntry() ? ((_guid >> 24) & 0x0000000000FFFFFF) : 0; }

    LowType GetCounter() const {
        return HasEntry()
               ? LowType(_guid & 0x0000000000FFFFFF)
               : LowType(_guid & 0x00000000FFFFFFFF);
    }

    static LowType GetMaxCounter(HighGuid high) {
        return HasEntry(high)
               ? LowType(0x00FFFFFF)
               : LowType(0xFFFFFFFF);
    }

    ObjectGuid::LowType GetMaxCounter() const { return GetMaxCounter(GetHigh()); }

    bool IsEmpty() const { return _guid == 0; }

    bool IsCreature() const { return GetHigh() == HighGuid::Unit; }

    bool IsPet() const { return GetHigh() == HighGuid::Pet; }

    bool IsVehicle() const { return GetHigh() == HighGuid::Vehicle; }

    bool IsCreatureOrPet() const { return IsCreature() || IsPet(); }

    bool IsCreatureOrVehicle() const { return IsCreature() || IsVehicle(); }

    bool IsAnyTypeCreature() const { return IsCreature() || IsPet() || IsVehicle(); }

    bool IsPlayer() const { return !IsEmpty() && GetHigh() == HighGuid::Player; }

    bool IsUnit() const { return IsAnyTypeCreature() || IsPlayer(); }

    bool IsItem() const { return GetHigh() == HighGuid::Item; }

    bool IsGameObject() const { return GetHigh() == HighGuid::GameObject; }

    bool IsDynamicObject() const { return GetHigh() == HighGuid::DynamicObject; }

    bool IsCorpse() const { return GetHigh() == HighGuid::Corpse; }

    bool IsTransport() const { return GetHigh() == HighGuid::Transport; }

    bool IsMOTransport() const { return GetHigh() == HighGuid::Mo_Transport; }

    bool IsAnyTypeGameObject() const { return IsGameObject() || IsTransport() || IsMOTransport(); }

    bool IsInstance() const { return GetHigh() == HighGuid::Instance; }

    bool IsGroup() const { return GetHigh() == HighGuid::Group; }

    static TypeID GetTypeId(HighGuid high) {
        switch (high) {
            case HighGuid::Item:
                return TYPEID_ITEM;
            case HighGuid::Unit:
                return TYPEID_UNIT;
            case HighGuid::Pet:
                return TYPEID_UNIT;
            case HighGuid::Player:
                return TYPEID_PLAYER;
            case HighGuid::GameObject:
                return TYPEID_GAMEOBJECT;
            case HighGuid::DynamicObject:
                return TYPEID_DYNAMICOBJECT;
            case HighGuid::Corpse:
                return TYPEID_CORPSE;
            case HighGuid::Mo_Transport:
                return TYPEID_GAMEOBJECT;
            case HighGuid::Vehicle:
                return TYPEID_UNIT;
            case HighGuid::Instance:
            case HighGuid::Group:
            default:
                return TYPEID_OBJECT;
        }
    }

    TypeID GetTypeId() const { return GetTypeId(GetHigh()); }

    bool operator!() const { return IsEmpty(); }

    bool operator==(ObjectGuid const &right) const = default;

    std::strong_ordering operator<=>(ObjectGuid const &right) const = default;

    static std::string_view GetTypeName(HighGuid high);

    std::string_view GetTypeName() const { return !IsEmpty() ? GetTypeName(GetHigh()) : "None"; }

    std::string ToString() const;

    std::string ToHexString() const;

private:
    static bool HasEntry(HighGuid high) {
        switch (high) {
            case HighGuid::Item:
            case HighGuid::Player:
            case HighGuid::DynamicObject:
            case HighGuid::Corpse:
            case HighGuid::Mo_Transport:
            case HighGuid::Instance:
            case HighGuid::Group:
                return false;
            case HighGuid::GameObject:
            case HighGuid::Transport:
            case HighGuid::Unit:
            case HighGuid::Pet:
            case HighGuid::Vehicle:
            default:
                return true;
        }
    }

    bool HasEntry() const { return HasEntry(GetHigh()); }

    static ObjectGuid Global(HighGuid type, LowType counter);

    static ObjectGuid MapSpecific(HighGuid type, uint32_t entry, LowType counter);

    explicit ObjectGuid(uint32_t const &) = delete;

    ObjectGuid(HighGuid, uint32_t, uint64_t counter) = delete;

    ObjectGuid(HighGuid, uint64_t counter) = delete;

    uint64_t _guid;
};

using GuidSet = std::set<ObjectGuid>;
using GuidList = std::list<ObjectGuid>;
using GuidVector = std::vector<ObjectGuid>;
using GuidUnorderedSet = std::unordered_set<ObjectGuid>;

namespace std {
    template<>
    struct hash<ObjectGuid> {
    public:
        size_t operator()(ObjectGuid const &key) const {
            return std::hash<uint64_t>()(key.GetRawValue());
        }
    };
}