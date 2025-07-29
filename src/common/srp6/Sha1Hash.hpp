#pragma once

#include <openssl/sha.h>
#include <vector>
#include <cstdint>

// Закрываем deprecated warning для OpenSSL 3.x
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

class Sha1Hash {
public:
    Sha1Hash() {
        SHA1_Init(&ctx_);
    }

    void Update(const uint8_t* data, size_t size) {
        SHA1_Update(&ctx_, data, size);
    }

    void Update(const std::vector<uint8_t>& data) {
        Update(data.data(), data.size());
    }

    std::vector<uint8_t> Finalize() {
        std::vector<uint8_t> hash(SHA_DIGEST_LENGTH);
        SHA1_Final(hash.data(), &ctx_);
        return hash;
    }

private:
    SHA_CTX ctx_;
};

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif
