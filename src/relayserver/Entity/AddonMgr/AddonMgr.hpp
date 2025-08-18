#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include <optional>
#include <vector>

class RelayServer;

struct SavedAddon {
    std::string Name;
    uint32_t CRC;
};

struct BannedAddon {
    uint32_t Id;
    std::array<uint8_t, 16> NameMD5;
    std::array<uint8_t, 16> VersionMD5;
    uint32_t Timestamp;
};

class AddonMgr {
public:
    explicit AddonMgr(std::shared_ptr<RelayServer> server) : server_(std::move(server)) {}

    ~AddonMgr();

    void loadFromDB();
    std::optional<SavedAddon> getAddonInfo(const std::string& name) const;
    const std::vector<BannedAddon>& getBannedAddons() const { return bannedAddonsVector_; }

private:
    std::shared_ptr<RelayServer> server_;

    std::unordered_map<std::string, SavedAddon> knownAddons_;
    std::vector<BannedAddon> bannedAddonsVector_; // Для быстрого доступа по порядку
};