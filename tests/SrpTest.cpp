#include <catch2/catch.hpp>

#include <array>
#include <string>
#include <iostream>
#include "Cryptography/srp6/SRP6.hpp"

using namespace Crypto;

static const std::string TEST_USERNAME = "TESTUSER"; // должен быть upper-case!
static const std::string TEST_PASSWORD = "StrongPassword123!";

TEST_CASE("SRP6: registration and verification", "[SRP6]") {
    // ===== Регистрация =====
    auto [salt, verifier] = SRP6::MakeRegistrationData(TEST_USERNAME, TEST_PASSWORD);

    REQUIRE(salt.size() == SRP6::SALT_LENGTH);
    REQUIRE(verifier.size() == SRP6::VERIFIER_LENGTH);

    // ===== Проверка входа =====
    bool ok = SRP6::CheckLogin(TEST_USERNAME, TEST_PASSWORD, salt, verifier);
    REQUIRE(ok);

    // ===== Проверка с неправильным паролем =====
    bool bad = SRP6::CheckLogin(TEST_USERNAME, "WrongPassword", salt, verifier);
    REQUIRE_FALSE(bad);
    std::cout << "✅ 'SRP6: registration and verification\n";
}

TEST_CASE("SRP6: VerifyChallengeResponse single use", "[SRP6]") {
    auto [salt, verifier] = SRP6::MakeRegistrationData(TEST_USERNAME, TEST_PASSWORD);

    SRP6 server(TEST_USERNAME, salt, verifier);

    SRP6::EphemeralKey A = {};
    SHA1::Digest clientM = {};

    // Первый вызов — должен пройти (результат может быть nullopt, это нормально)
    auto K = server.VerifyChallengeResponse(A, clientM);

    // Второй вызов — обязан выкинуть!
    REQUIRE_THROWS_AS(server.VerifyChallengeResponse(A, clientM), std::runtime_error);
    std::cout << "✅ 'SRP6: VerifyChallengeResponse single use\n";
}
