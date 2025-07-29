#pragma once

#include <openssl/bn.h>
#include <vector>
#include <string>
#include <stdexcept>

class BigNumber {
public:
    BigNumber() {
        bn_ = BN_new();
        if (!bn_) {
            throw std::runtime_error("BN_new failed");
        }
    }

    BigNumber(const BigNumber& other) {
        bn_ = BN_dup(other.bn_);
        if (!bn_) {
            throw std::runtime_error("BN_dup failed");
        }
    }

    BigNumber& operator=(const BigNumber& other) {
        if (this != &other) {
            BIGNUM* tmp = BN_dup(other.bn_);
            if (!tmp) {
                throw std::runtime_error("BN_dup failed");
            }
            BN_free(bn_);
            bn_ = tmp;
        }
        return *this;
    }

    ~BigNumber() {
        BN_free(bn_);
    }

    void SetHexStr(const std::string& hex) {
        if (!BN_hex2bn(&bn_, hex.c_str())) {
            throw std::runtime_error("BN_hex2bn failed");
        }
    }

    void SetDword(uint32_t val) {
        if (!BN_set_word(bn_, val)) {
            throw std::runtime_error("BN_set_word failed");
        }
    }

    void SetBinary(const uint8_t* data, size_t size) {
        BIGNUM* tmp = BN_bin2bn(data, size, nullptr);
        if (!tmp) {
            throw std::runtime_error("BN_bin2bn failed");
        }
        BN_free(bn_);
        bn_ = tmp;
    }

    void SetRand(int bits) {
        if (!BN_rand(bn_, bits, -1, 0)) {
            throw std::runtime_error("BN_rand failed");
        }
    }

    std::vector<uint8_t> AsBytes() const {
        int size = BN_num_bytes(bn_);
        std::vector<uint8_t> buf(size);
        if (BN_bn2bin(bn_, buf.data()) != size) {
            throw std::runtime_error("BN_bn2bin failed");
        }
        return buf;
    }

    BigNumber ModExp(const BigNumber& exp, const BigNumber& mod) const {
        BigNumber res;
        BN_CTX* ctx = BN_CTX_new();
        if (!ctx) throw std::runtime_error("BN_CTX_new failed");
        if (!BN_mod_exp(res.bn_, bn_, exp.bn_, mod.bn_, ctx)) {
            BN_CTX_free(ctx);
            throw std::runtime_error("BN_mod_exp failed");
        }
        BN_CTX_free(ctx);
        return res;
    }

    BigNumber ModMul(const BigNumber& val, const BigNumber& mod) const {
        BigNumber res;
        BN_CTX* ctx = BN_CTX_new();
        if (!ctx) throw std::runtime_error("BN_CTX_new failed");
        if (!BN_mod_mul(res.bn_, bn_, val.bn_, mod.bn_, ctx)) {
            BN_CTX_free(ctx);
            throw std::runtime_error("BN_mod_mul failed");
        }
        BN_CTX_free(ctx);
        return res;
    }

    BigNumber ModAdd(const BigNumber& val, const BigNumber& mod) const {
        BigNumber res;
        BN_CTX* ctx = BN_CTX_new();
        if (!ctx) throw std::runtime_error("BN_CTX_new failed");
        if (!BN_mod_add(res.bn_, bn_, val.bn_, mod.bn_, ctx)) {
            BN_CTX_free(ctx);
            throw std::runtime_error("BN_mod_add failed");
        }
        BN_CTX_free(ctx);
        return res;
    }

    BigNumber ModSub(const BigNumber& val, const BigNumber& mod) const {
        BigNumber res;
        BN_CTX* ctx = BN_CTX_new();
        if (!ctx) throw std::runtime_error("BN_CTX_new failed");
        if (!BN_mod_sub(res.bn_, bn_, val.bn_, mod.bn_, ctx)) {
            BN_CTX_free(ctx);
            throw std::runtime_error("BN_mod_sub failed");
        }
        BN_CTX_free(ctx);
        return res;
    }

    bool IsZero() const {
        return BN_is_zero(bn_) != 0;
    }

    bool operator==(const BigNumber& other) const {
        return BN_cmp(bn_, other.bn_) == 0;
    }

    bool operator!=(const BigNumber& other) const {
        return !(*this == other);
    }

    BIGNUM* GetBIGNUM() const {
        return bn_;
    }

    static BigNumber FromBytes(const uint8_t* data, size_t size) {
        BigNumber bn;
        bn.SetBinary(data, size);
        return bn;
    }

private:
    BIGNUM* bn_;
};
