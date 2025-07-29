#pragma once

#include "BigNumber.hpp"
#include "Sha1Hash.hpp"
#include <vector>
#include <string>
#include <stdexcept>

class SRP6 {
public:
    SRP6() {
        // N и g — TrinityCore эталон
        N_.SetHexStr("B79B3E2A87823CAB8F5EBFBF8EB1010853506298B5BADBD5B53E1895E644B89");
        g_.SetDword(7);

        // k = SHA1(N | PAD(g))
        auto N_bytes = N_.AsBytes();
        auto g_bytes = PadTo(N_bytes.size(), g_);
        Sha1Hash sha;
        sha.Update(N_bytes);
        sha.Update(g_bytes);
        auto hash = sha.Finalize();
        k_.SetBinary(hash.data(), hash.size());
    }

    void LoadVerifier(const std::vector<uint8_t>& salt, const std::vector<uint8_t>& verifier) {
        salt_ = salt;
        v_.SetBinary(verifier.data(), verifier.size());
    }

    void GenerateServerEphemeral() {
        b_.SetRand(256);
        BigNumber gb = g_.ModExp(b_, N_);
        BigNumber kv = k_.ModMul(v_, N_);
        B_ = kv.ModAdd(gb, N_);
    }

    std::vector<uint8_t> GetBBytes() const {
        return B_.AsBytes();
    }

    std::vector<uint8_t> GetNBytes() const {
        return N_.AsBytes();
    }

    uint8_t GetGenerator() const {
        return 7;
    }

    std::vector<uint8_t> GetSalt() const {
        return salt_;
    }

    // --- u = SHA1(A | B)
    BigNumber CalculateU(const BigNumber& A, const BigNumber& B) {
        auto A_bytes = A.AsBytes();
        auto B_bytes = B.AsBytes();
        Sha1Hash sha;
        sha.Update(A_bytes);
        sha.Update(B_bytes);
        auto u_bytes = sha.Finalize();

        BigNumber u;
        u.SetBinary(u_bytes.data(), u_bytes.size());
        return u;
    }

    // S = (A * v^u) ^ b mod N
    BigNumber CalculateS(const BigNumber& A) {
        BigNumber u = CalculateU(A, B_);
        BigNumber vu = v_.ModExp(u, N_);
        BigNumber Avu = A.ModMul(vu, N_);
        S_ = Avu.ModExp(b_, N_);
        return S_;
    }

    std::vector<uint8_t> CalculateSessionKey() {
        auto S_bytes = S_.AsBytes();
        Sha1Hash sha;
        sha.Update(S_bytes);
        session_key_ = sha.Finalize();
        return session_key_;
    }

private:
    std::vector<uint8_t> PadTo(size_t size, const BigNumber& num) {
        auto bytes = num.AsBytes();
        if (bytes.size() >= size)
            return bytes;
        std::vector<uint8_t> padded(size, 0);
        std::copy_backward(bytes.begin(), bytes.end(), padded.end());
        return padded;
    }

    BigNumber N_;
    BigNumber g_;
    BigNumber k_;
    BigNumber v_;
    BigNumber b_;
    BigNumber B_;
    BigNumber S_;
    std::vector<uint8_t> salt_;
    std::vector<uint8_t> session_key_;
};
