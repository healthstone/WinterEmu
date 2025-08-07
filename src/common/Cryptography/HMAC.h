#include "CryptoConstants.hpp"
#include "CryptoHash.hpp"

#include <array>
#include <string>
#include <string_view>

class BigNumber;

namespace Impl {
    template<GenericHashImpl::HashCreator HashCreator, size_t DigestLength>
    class GenericHMAC {
    public:
        static constexpr size_t DIGEST_LENGTH = DigestLength;
        using Digest = std::array<uint8_t, DIGEST_LENGTH>;

        template<typename Container>
        static Digest GetDigestOf(Container const &seed, uint8_t const *data, size_t len) {
            GenericHMAC hash(seed);
            hash.UpdateData(data, len);
            hash.Finalize();
            return hash.GetDigest();
        }

        template<typename Container, typename... Ts>
        static auto GetDigestOf(Container const &seed,
                                Ts &&... pack) -> std::enable_if_t<!(std::is_integral_v<std::decay_t<Ts>> || ...), Digest> {
            GenericHMAC hash(seed);
            (hash.UpdateData(std::forward<Ts>(pack)), ...);
            hash.Finalize();
            return hash.GetDigest();
        }

        GenericHMAC(uint8_t const *seed, size_t len) : _ctx(GenericHashImpl::MakeCTX()),
                                                       _key(EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, nullptr, seed, len)) {
            int result = EVP_DigestSignInit(_ctx, nullptr, HashCreator(), nullptr, _key);
            if (result != 1) {
                throw std::runtime_error("HMAC::GenericHMAC(uint8_t const* seed, size_t len): result != 1");
            }
        }

        template<typename Container>
        GenericHMAC(Container const &container) : GenericHMAC(std::data(container), std::size(container)) {}

        GenericHMAC(GenericHMAC const &right) : _ctx(GenericHashImpl::MakeCTX()) {
            *this = right;
        }

        GenericHMAC(GenericHMAC &&right) noexcept {
            *this = std::move(right);
        }

        ~GenericHMAC() {
            GenericHashImpl::DestroyCTX(_ctx);
            _ctx = nullptr;
            EVP_PKEY_free(_key);
            _key = nullptr;
        }

        GenericHMAC &operator=(GenericHMAC const &right) {
            if (this == &right)
                return *this;

            int result = EVP_MD_CTX_copy_ex(_ctx, right._ctx);
            if (result != 1) {
                throw std::runtime_error("HMAC::GenericHMAC& operator=(GenericHMAC const& right): result != 1");
            }
            _key = right._key;      // EVP_PKEY uses reference counting internally, just copy the pointer
            EVP_PKEY_up_ref(
                    _key);  // Bump reference count for PKEY, as every instance of this class holds two references to PKEY and destructor decrements it twice
            _digest = right._digest;
            return *this;
        }

        GenericHMAC &operator=(GenericHMAC &&right) noexcept {
            if (this == &right)
                return *this;

            _ctx = std::exchange(right._ctx, GenericHashImpl::MakeCTX());
            _key = std::exchange(right._key, EVP_PKEY_new());
            _digest = std::exchange(right._digest, Digest{});
            return *this;
        }

        void UpdateData(uint8_t const *data, size_t len) {
            int result = EVP_DigestSignUpdate(_ctx, data, len);
            if (result != 1) {
                throw std::runtime_error("HMAC::UpdateData: result != 1");
            }
        }

        void UpdateData(std::string_view str) { UpdateData(reinterpret_cast<uint8_t const *>(str.data()), str.size()); }

        void UpdateData(std::string const &str) {
            UpdateData(std::string_view(str));
        } /* explicit overload to avoid using the container template */
        void UpdateData(char const *str) {
            UpdateData(std::string_view(str));
        } /* explicit overload to avoid using the container template */
        template<typename Container>
        void UpdateData(Container const &c) { UpdateData(std::data(c), std::size(c)); }

        void Finalize() {
            size_t length = DIGEST_LENGTH;
            int result = EVP_DigestSignFinal(_ctx, _digest.data(), &length);
            if (result != 1) {
                throw std::runtime_error("HMAC::Finalize: result != 1");
            }
            if (length != DIGEST_LENGTH) {
                throw std::runtime_error("HMAC::Finalize: length != DIGEST_LENGTH");
            }
        }

        Digest const &GetDigest() const { return _digest; }

    private:
        EVP_MD_CTX *_ctx;
        EVP_PKEY *_key;
        Digest _digest = {};
    };
}

namespace Crypto {
    using HMAC_SHA1 = Impl::GenericHMAC<EVP_sha1, Constants::SHA1_DIGEST_LENGTH_BYTES>;
    using HMAC_SHA256 = Impl::GenericHMAC<EVP_sha256, Constants::SHA256_DIGEST_LENGTH_BYTES>;
}