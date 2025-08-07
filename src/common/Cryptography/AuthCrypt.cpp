#include "AuthCrypt.hpp"
#include "BigNumber.hpp"
#include "HMAC.h"

#include <cstring>

AuthCrypt::AuthCrypt() :
        _initialized(false) {}

void AuthCrypt::Init(SessionKey const &K) {
    uint8_t ServerEncryptionKey[] = {0xCC, 0x98, 0xAE, 0x04, 0xE8, 0x97, 0xEA, 0xCA, 0x12, 0xDD, 0xC0, 0x93, 0x42, 0x91,
                                     0x53, 0x57};
    _serverEncrypt.Init(Crypto::HMAC_SHA1::GetDigestOf(ServerEncryptionKey, K));
    uint8_t ServerDecryptionKey[] = {0xC2, 0xB3, 0x72, 0x3C, 0xC6, 0xAE, 0xD9, 0xB5, 0x34, 0x3C, 0x53, 0xEE, 0x2F, 0x43,
                                     0x67, 0xCE};
    _clientDecrypt.Init(Crypto::HMAC_SHA1::GetDigestOf(ServerDecryptionKey, K));

    // Drop first 1024 bytes, as WoW uses ARC4-drop1024.
    std::array<uint8_t, 1024> syncBuf;
    _serverEncrypt.UpdateData(syncBuf);
    _clientDecrypt.UpdateData(syncBuf);

    _initialized = true;
}

void AuthCrypt::DecryptRecv(uint8_t *data, size_t len) {
    if (!_initialized) {
        throw std::runtime_error("AuthCrypt: DecryptRecv _initialized = false");
    }
    _clientDecrypt.UpdateData(data, len);
}

void AuthCrypt::EncryptSend(uint8_t *data, size_t len) {
    if (!_initialized) {
        throw std::runtime_error("AuthCrypt: EncryptSend _initialized = false");
    }
    _serverEncrypt.UpdateData(data, len);
}
