#include <stdexcept>
#include "ARC4.hpp"

Crypto::ARC4::ARC4() : _ctx(EVP_CIPHER_CTX_new()) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    _cipher = EVP_CIPHER_fetch(nullptr, "RC4", nullptr);
#else
    EVP_CIPHER const* _cipher = EVP_rc4();
#endif

    EVP_CIPHER_CTX_init(_ctx);
    int result = EVP_EncryptInit_ex(_ctx, _cipher, nullptr, nullptr, nullptr);
    if (result != 1) {
        throw std::runtime_error("ARC4::ARC4(): result != 1");
    }
}

Crypto::ARC4::~ARC4() {
    EVP_CIPHER_CTX_free(_ctx);

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    EVP_CIPHER_free(_cipher);
#endif
}

void Crypto::ARC4::Init(uint8_t const *seed, size_t len) {
    int result1 = EVP_CIPHER_CTX_set_key_length(_ctx, len);
    if (result1 != 1) {
        throw std::runtime_error("ARC4::Init: result1 != 1");
    }
    int result2 = EVP_EncryptInit_ex(_ctx, nullptr, nullptr, seed, nullptr);
    if (result2 != 1) {
        throw std::runtime_error("ARC4::Init: result2 != 1");
    }
}

void Crypto::ARC4::UpdateData(uint8_t *data, size_t len) {
    int outlen = 0;
    int result1 = EVP_EncryptUpdate(_ctx, data, &outlen, data, len);
    if (result1 != 1) {
        throw std::runtime_error("ARC4::UpdateData: result1 != 1");
    }
    int result2 = EVP_EncryptFinal_ex(_ctx, data, &outlen);
    if (result2 != 1) {
        throw std::runtime_error("ARC4::UpdateData: result2 != 1");
    }
}
