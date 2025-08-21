#include "AddonMgr.hpp"
#include "src/relayserver/RelayServer.hpp"
#include "Time/Timer.hpp"
#include "Cryptography/CryptoHash.hpp"

AddonMgr::~AddonMgr() {
    cleanUpBeforeDelete();
}

void AddonMgr::cleanUpBeforeDelete() {
    knownAddons_.clear();
    bannedAddonsVector_.clear();
}

void AddonMgr::loadFromDB() {
    knownAddons_.clear();
    bannedAddonsVector_.clear();

    auto log = Logger::get();
    try {
        uint32_t oldMSTime1 = getMSTime();

        auto stmt1 = PreparedStatement("SELECT_ADDONS");
        auto rows1 = server_->db()->execute_sync_many<AddonRow>(stmt1);
        for (const auto &row: rows1) {
            knownAddons_[row.name] = {row.name, row.crc};
        }
        log->info(">>> AddonMgr: loaded {} addons in {} ms",
                  rows1.size(), GetMSTimeDiffToNow(oldMSTime1));

        uint32_t oldMSTime2 = getMSTime();
        auto stmt2 = PreparedStatement("SELECT_BANNED_ADDONS");
        auto rows2 = server_->db()->execute_sync_many<BannedAddonRow>(stmt2);
        for (const auto &row: rows2) {
            BannedAddon banned;
            banned.Id = row.id;
            banned.Timestamp = row.timestamp;
            banned.NameMD5 = Crypto::MD5::GetDigestOf(row.name);
            banned.VersionMD5 = Crypto::MD5::GetDigestOf(row.version);

            bannedAddonsVector_.push_back(banned);
        }

        // Сортируем по timestamp для бинарного поиска
        std::sort(bannedAddonsVector_.begin(), bannedAddonsVector_.end(),
                  [](const BannedAddon& a, const BannedAddon& b) {
                      return a.Timestamp < b.Timestamp;
                  });

        log->info(">>> AddonMgr: loaded {} banned addons in {} ms",
                  rows2.size(), GetMSTimeDiffToNow(oldMSTime2));
    }
    catch (const std::exception &ex) {
        log->error("AddonMgr::loadFromDB failed: {}", ex.what());
    }
}

std::optional<SavedAddon> AddonMgr::getAddonInfo(const std::string& name) const {
    auto it = knownAddons_.find(name);
    return it != knownAddons_.end()
           ? std::make_optional(it->second)
           : std::nullopt;
}