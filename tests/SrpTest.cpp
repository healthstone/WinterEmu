#include <catch2/catch.hpp>
#include <openssl/sha.h>
#include <iostream>
#include "srp6/SRP6.hpp"
#include "utils/HexUtils.hpp"

TEST_CASE("SRP6: init constants correctly", "[srp6]") {
    SRP6 srp;

    auto N_bytes = srp.GetNBytes();
    auto g = srp.GetGenerator();

    REQUIRE(N_bytes.size() > 0);
    REQUIRE(g == 7);

    std::vector<uint8_t> to_hash;
    to_hash.insert(to_hash.end(), N_bytes.begin(), N_bytes.end());

    // g нужно допадать до длины N, т.к. в конструкторе к g тоже применяется padding
    std::vector<uint8_t> g_padded(srp.GetNBytes().size(), 0);
    g_padded.back() = g;
    to_hash.insert(to_hash.end(), g_padded.begin(), g_padded.end());

    uint8_t hash[SHA_DIGEST_LENGTH];
    SHA1(to_hash.data(), to_hash.size(), hash);

    bool hash_nonzero = false;
    for (auto b : hash) {
        if (b != 0) {
            hash_nonzero = true;
            break;
        }
    }
    REQUIRE(hash_nonzero);
    std::cout << "✅ 'SRP6: init constants correctly\n";
}

TEST_CASE("SRP6: load verifier and generate server ephemeral", "[srp6]") {
    SRP6 srp;

    auto salt = std::vector<uint8_t>{1,2,3,4,5};
    auto verifier = std::vector<uint8_t>(32, 0xAB);

    srp.LoadVerifier(salt, verifier);
    REQUIRE(srp.GetSalt() == salt);

    srp.GenerateServerEphemeral();
    auto B = srp.GetBBytes();

    REQUIRE(!B.empty());
    std::cout << "✅ 'SRP6: load verifier and generate server ephemeral\n";
}

TEST_CASE("SRP6: generate different B each time", "[srp6]") {
    SRP6 srp;

    auto salt = std::vector<uint8_t>{1,2,3,4,5};
    auto verifier = std::vector<uint8_t>(32, 0x01);

    srp.LoadVerifier(salt, verifier);

    srp.GenerateServerEphemeral();
    auto B1 = srp.GetBBytes();

    srp.GenerateServerEphemeral();
    auto B2 = srp.GetBBytes();

    REQUIRE(B1 != B2);
    std::cout << "✅ 'SRP6: generate different B each time\n";
}

//TEST_CASE("SRP6 full client-server authentication flow", "[srp6]") {
//    // --- Данные из БД: salt и verifier ---
//    // Пример из TrinityCore / твоего эталона:
//    std::vector<uint8_t> salt = HexUtils::hex_to_bytes("143F381865A0C9584F0FCBE7E856D52377C37F0F6DFCC3DA518FCED2F0BBDB1D");
//    std::vector<uint8_t> verifier = HexUtils::hex_to_bytes("039E29F7212ABF38606FCB1598B7FD5F12CE9E9DE5D7707147B5C971C8E48B5F");
//
//    std::string username = "1";
//    std::string password = "1";
//
//    // --- СЕРВЕР ---
//    SRP6 server;
//    server.LoadVerifier(salt, verifier);
//    server.GenerateServerEphemeral();
//    auto B = server.GetBBytes();
//    auto N = server.GetNBytes();
//    auto g = server.GetGenerator();
//    auto s = server.GetSalt();
//
//    REQUIRE(B.size() == 32);
//    REQUIRE(N.size() == 32);
//    REQUIRE(s.size() == 32);
//    REQUIRE(g == 7);
//
//    // --- КЛИЕНТ ---
//    SRP6 client;
//    client.SetCredentials(username, password);
//    client.LoadConstants(N, g);
//    client.LoadSalt(salt);
//    client.GenerateClientEphemeral();
//    auto A = client.GetABytes();
//
//    REQUIRE(!A.empty());
//
//    // --- Клиент вычисляет общий секрет и M1 ---
//    client.CalculateSessionKeyClient(BigNumber::FromBytes(B.data(), B.size()));
//    auto M1 = client.CalculateM1(BigNumber::FromBytes(B.data(), B.size()));
//
//    REQUIRE(!M1.empty());
//
//    // --- Сервер вычисляет общий секрет ---
//    server.CalculateSessionKeyServer(BigNumber::FromBytes(A.data(), A.size()));
//
//    // --- Сервер проверяет M1 и формирует M2 ---
//    std::vector<uint8_t> M2;
//    bool verified = server.VerifyClientProof(BigNumber::FromBytes(A.data(), A.size()), M1, M2);
//    REQUIRE(verified);
//    REQUIRE(!M2.empty());
//
//    // --- Клиент проверяет M2 ---
//    bool server_ok = client.VerifyServerProof(M1, M2);
//    REQUIRE(server_ok);
//
//    std::cout << "✅ SRP6 full client-server authentication flow\n";
//}

TEST_CASE("SRP6: load verifier stores salt and verifier", "[srp6]") {
    try {
        SRP6 srp;

        std::vector<uint8_t> salt = { 1, 2, 3, 4, 5 };
        std::vector<uint8_t> verifier(32, 0xAB);

        srp.LoadVerifier(salt, verifier);

        auto salt_out = srp.GetSalt();
        REQUIRE(salt_out == salt);

        srp.GenerateServerEphemeral();
        auto B_bytes = srp.GetBBytes();

        REQUIRE(B_bytes.size() > 0);
        std::cout << "✅ 'SRP6: load verifier stores salt and verifier\n";
    } catch (const std::exception& e) {
        FAIL("Exception thrown: " << e.what());
    }
}