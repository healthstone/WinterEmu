#include <stdexcept>
#include <cstring>
#include <openssl/provider.h>
#include "ARC4.hpp"

namespace Crypto {

    ARC4::ARC4() : _ctx(EVP_CIPHER_CTX_new())
    {
        if (!_ctx)
            throw std::runtime_error("ARC4::ARC4(): EVP_CIPHER_CTX_new failed");

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
        // OpenSSL 3.x требует загрузить legacy-провайдер, где находится RC4
        if (!OSSL_PROVIDER_load(nullptr, "legacy"))
            throw std::runtime_error("ARC4::ARC4(): failed to load legacy provider");
        if (!OSSL_PROVIDER_load(nullptr, "default"))
            throw std::runtime_error("ARC4::ARC4(): failed to load default provider");

        _cipher = EVP_CIPHER_fetch(nullptr, "RC4", nullptr);
        if (!_cipher)
            throw std::runtime_error("ARC4::ARC4(): cannot fetch RC4 cipher (legacy provider missing)");

        if (EVP_EncryptInit_ex(_ctx, _cipher, nullptr, nullptr, nullptr) != 1)
            throw std::runtime_error("ARC4::ARC4(): EVP_EncryptInit_ex failed");
#else
        const EVP_CIPHER* cipher = EVP_rc4();
    if (!cipher)
        throw std::runtime_error("ARC4::ARC4(): EVP_rc4() returned nullptr");

    EVP_CIPHER_CTX_init(_ctx);
    if (EVP_EncryptInit_ex(_ctx, cipher, nullptr, nullptr, nullptr) != 1)
        throw std::runtime_error("ARC4::ARC4(): EVP_EncryptInit_ex failed");
#endif
    }

    ARC4::~ARC4()
    {
        if (_ctx)
            EVP_CIPHER_CTX_free(_ctx);

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
        if (_cipher)
            EVP_CIPHER_free(_cipher);
#endif
    }

    void ARC4::Init(uint8_t const* seed, size_t len)
    {
        if (!seed || len == 0)
            throw std::invalid_argument("ARC4::Init(): invalid key data");

        if (EVP_CIPHER_CTX_set_key_length(_ctx, static_cast<int>(len)) != 1)
            throw std::runtime_error("ARC4::Init(): EVP_CIPHER_CTX_set_key_length failed");

        if (EVP_EncryptInit_ex(_ctx, nullptr, nullptr, seed, nullptr) != 1)
            throw std::runtime_error("ARC4::Init(): EVP_EncryptInit_ex failed");
    }

    void ARC4::UpdateData(uint8_t* data, size_t len)
    {
        if (!data || len == 0)
            return;

        int outlen = 0;
        if (EVP_EncryptUpdate(_ctx, data, &outlen, data, static_cast<int>(len)) != 1)
            throw std::runtime_error("ARC4::UpdateData(): EVP_EncryptUpdate failed");

        // Для RC4 выходной размер всегда совпадает с входным
        if (static_cast<size_t>(outlen) != len)
            throw std::runtime_error("ARC4::UpdateData(): outlen != len");
    }

} // namespace Crypto
