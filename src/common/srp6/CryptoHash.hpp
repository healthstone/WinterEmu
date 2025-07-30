#ifndef CRYPTOHASH_H
#define CRYPTOHASH_H

#include "CryptoConstants.hpp"
#include <array>
#include <string>
#include <string_view>
#include <utility>
#include <openssl/evp.h>

class BigNumber;

namespace Impl
{
    struct GenericHashImpl
    {
        typedef EVP_MD const* (*HashCreator)();

        static EVP_MD_CTX* MakeCTX() noexcept { return EVP_MD_CTX_new(); }
        static void DestroyCTX(EVP_MD_CTX* ctx) { EVP_MD_CTX_free(ctx); }
    };

    template <GenericHashImpl::HashCreator HashCreator, size_t DigestLength>
    class GenericHash
    {
    public:
        static constexpr size_t DIGEST_LENGTH = DigestLength;
        using Digest = std::array<uint8_t, DIGEST_LENGTH>;

        static Digest GetDigestOf(uint8_t const* data, size_t len)
        {
            GenericHash hash;
            hash.UpdateData(data, len);
            hash.Finalize();
            return hash.GetDigest();
        }

        template <typename... Ts>
        static auto GetDigestOf(Ts&&... pack) -> std::enable_if_t<!(std::is_integral_v<std::decay_t<Ts>> || ...), Digest>
        {
            GenericHash hash;
            (hash.UpdateData(std::forward<Ts>(pack)), ...);
            hash.Finalize();
            return hash.GetDigest();
        }

        GenericHash() : _ctx(GenericHashImpl::MakeCTX())
        {
            int result = EVP_DigestInit_ex(_ctx, HashCreator(), nullptr);
            if (result != 1) {
                throw std::runtime_error("GenericHash() : _ctx(GenericHashImpl::MakeCTX()): result != 1");
            }
        }

        GenericHash(GenericHash const& right) : _ctx(GenericHashImpl::MakeCTX())
        {
            *this = right;
        }

        GenericHash(GenericHash&& right) noexcept
        {
            *this = std::move(right);
        }

        ~GenericHash()
        {
            if (!_ctx)
                return;
            GenericHashImpl::DestroyCTX(_ctx);
            _ctx = nullptr;
        }

        GenericHash& operator=(GenericHash const& right)
        {
            if (this == &right)
                return *this;

            int result = EVP_MD_CTX_copy_ex(_ctx, right._ctx);
            if (result != 1) {
                throw std::runtime_error("GenericHash& operator=(GenericHash const& right): result != 1");
            }
            _digest = right._digest;
            return *this;
        }

        GenericHash& operator=(GenericHash&& right) noexcept
        {
            if (this == &right)
                return *this;

            _ctx = std::exchange(right._ctx, GenericHashImpl::MakeCTX());
            _digest = std::exchange(right._digest, Digest{});
            return *this;
        }

        void UpdateData(uint8_t const* data, size_t len)
        {
            int result = EVP_DigestUpdate(_ctx, data, len);
            if (result != 1) {
                throw std::runtime_error("void UpdateData(uint8_t const* data, size_t len): result != 1");
            }
        }
        void UpdateData(std::string_view str) { UpdateData(reinterpret_cast<uint8_t const*>(str.data()), str.size()); }
        void UpdateData(std::string const& str) { UpdateData(std::string_view(str)); } /* explicit overload to avoid using the container template */
        void UpdateData(char const* str) { UpdateData(std::string_view(str)); } /* explicit overload to avoid using the container template */
        template <typename Container>
        void UpdateData(Container const& c) { UpdateData(std::data(c), std::size(c)); }

        void Finalize()
        {
            uint32_t length;
            int result = EVP_DigestFinal_ex(_ctx, _digest.data(), &length);
            if (result != 1) {
                throw std::runtime_error("void Finalize(): result != 1");
            }
            if (length != DIGEST_LENGTH) {
                throw std::runtime_error("void Finalize(): length != DIGEST_LENGTH");
            }
        }

        Digest const& GetDigest() const { return _digest; }

    private:
        EVP_MD_CTX* _ctx;
        Digest _digest = { };
    };
}

namespace Crypto
{
    using MD5 = Impl::GenericHash<EVP_md5, Constants::MD5_DIGEST_LENGTH_BYTES>;
    using SHA1 = Impl::GenericHash<EVP_sha1, Constants::SHA1_DIGEST_LENGTH_BYTES>;
    using SHA256 = Impl::GenericHash<EVP_sha256, Constants::SHA256_DIGEST_LENGTH_BYTES>;
}

#endif
