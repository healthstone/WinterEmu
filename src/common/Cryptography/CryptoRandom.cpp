#include "CryptoRandom.hpp"
#include <openssl/rand.h>
#include <stdexcept>

void Crypto::GetRandomBytes(uint8_t *buf, size_t len) {
    int result = RAND_bytes(buf, len);
    if (result != 1) {
        throw std::runtime_error(
                "Crypto::GetRandomBytes: Not enough randomness in OpenSSL's entropy pool. What in the world are you running on?");
    }
}
