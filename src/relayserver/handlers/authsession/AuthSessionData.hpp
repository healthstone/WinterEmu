#include <cstdint>
#include <string>
#include <vector>
#include "Cryptography/CryptoHash.hpp"

class AuthSessionData {
public:
    uint32_t client_build;
    uint32_t login_server_id;
    std::string accountName;            // Null-terminated string
    uint32_t login_server_type;
    uint32_t client_seed;
    uint32_t region_id;
    uint32_t battleground_id;
    uint32_t realm_id;
    uint64_t dos_response;
    Crypto::SHA1::Digest digest;
    std::vector<uint8_t> addonData;
};