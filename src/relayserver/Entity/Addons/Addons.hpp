#include <cstdint>

struct Addons {
    struct SecureAddonInfo {
        enum SecureAddonStatus : uint8_t {
            BANNED          = 0,
            SECURE_VISIBLE  = 1,
            SECURE_HIDDEN   = 2
        };

        std::string Name;
        SecureAddonStatus Status = BANNED;
        bool HasKey = false;
        uint32_t PublicKeyCrc = 0;
        uint32_t UrlCrc = 0;
    };

    static constexpr uint32_t MaxSecureAddons = 25;
    std::vector<SecureAddonInfo> SecureAddons;
    uint32_t LastBannedAddOnTimestamp = 0;
};