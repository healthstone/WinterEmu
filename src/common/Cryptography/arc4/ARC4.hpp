#include <array>
#include <openssl/evp.h>

namespace Crypto {
    class ARC4 {
    public:
        ARC4();

        ~ARC4();

        void Init(uint8_t const *seed, size_t len);

        template<typename Container>
        void Init(Container const &c) { Init(std::data(c), std::size(c)); }

        void UpdateData(uint8_t *data, size_t len);

        template<typename Container>
        void UpdateData(Container &c) { UpdateData(std::data(c), std::size(c)); }

    private:
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
        EVP_CIPHER *_cipher;
#endif
        EVP_CIPHER_CTX *_ctx;
    };
}