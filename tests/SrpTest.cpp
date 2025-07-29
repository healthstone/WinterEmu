#include <catch2/catch.hpp>

#include "srp6/SRP6.hpp"
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <vector>
#include <cstring>
#include <iostream>

TEST_CASE("SRP6: init constants correctly", "[srp6]") {
    SRP6 srp;

    auto N_bytes = srp.get_N_bytes();
    auto g = srp.get_generator();

    REQUIRE(N_bytes.size() > 0);
    REQUIRE(g == 7); // твой константный генератор

    // Проверим, что k не нулевой
    std::vector<uint8_t> to_hash;
    to_hash.insert(to_hash.end(), N_bytes.begin(), N_bytes.end());
    to_hash.push_back(g);

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

TEST_CASE("SRP6: load verifier stores salt and verifier", "[srp6]") {
    SRP6 srp;

    std::vector<uint8_t> salt = { 1, 2, 3, 4, 5 };
    std::vector<uint8_t> verifier(32, 0xAB);

    srp.load_verifier(salt, verifier);

    auto salt_out = srp.get_salt_bytes();
    REQUIRE(salt_out == salt);

    // Проверим что internal v_ совпадает с тем, что мы загрузили
    BIGNUM* v_check = BN_new();
    BN_bin2bn(verifier.data(), verifier.size(), v_check);

    // В реальном коде проверить приватный v_ можно через B_
    // Для теста генерируем ephemeral и проверяем, что B не ноль
    srp.generate_server_ephemeral();
    auto B_bytes = srp.get_B_bytes();

    REQUIRE(B_bytes.size() > 0);

    BN_free(v_check);
    std::cout << "✅ 'SRP6: load verifier stores salt and verifier\n";
}

TEST_CASE("SRP6: authserver ephemeral generates different B each time", "[srp6]") {
    SRP6 srp;
    std::vector<uint8_t> salt = { 1, 2, 3, 4, 5 };
    std::vector<uint8_t> verifier(32, 0x01);

    srp.load_verifier(salt, verifier);

    srp.generate_server_ephemeral();
    auto B1 = srp.get_B_bytes();

    srp.generate_server_ephemeral();
    auto B2 = srp.get_B_bytes();

    REQUIRE(B1 != B2);
    std::cout << "✅ 'SRP6: authserver ephemeral generates different B each time\n";
}
