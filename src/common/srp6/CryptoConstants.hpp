#ifndef CRYPTO_CONSTANTS_H
#define CRYPTO_CONSTANTS_H

namespace Crypto
{
    struct Constants
    {
        static constexpr size_t MD5_DIGEST_LENGTH_BYTES = 16;
        static constexpr size_t SHA1_DIGEST_LENGTH_BYTES = 20;
        static constexpr size_t SHA256_DIGEST_LENGTH_BYTES = 32;
    };
}

#endif
