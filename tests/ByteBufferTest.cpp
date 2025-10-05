#include <catch2/catch_all.hpp>
#include "packet/ByteBuffer.hpp"

TEST_CASE("ByteBuffer integer BE/LE read/write") {
    ByteBuffer buf;

    buf.write_uint16_be(0x1234);
    buf.write_uint16_le(0x1234);
    buf.write_uint32_be(0x12345678);
    buf.write_uint32_le(0x12345678);
    buf.write_uint64_be(0x1234567890ABCDEF);
    buf.write_uint64_le(0x1234567890ABCDEF);

    buf.reset_read();

    REQUIRE(buf.read_uint16_be() == 0x1234);
    REQUIRE(buf.read_uint16_le() == 0x1234);
    REQUIRE(buf.read_uint32_be() == 0x12345678);
    REQUIRE(buf.read_uint32_le() == 0x12345678);
    REQUIRE(buf.read_uint64_be() == 0x1234567890ABCDEF);
    REQUIRE(buf.read_uint64_le() == 0x1234567890ABCDEF);
    std::cout << "✅ 'ByteBuffer integer BE/LE read/write\n";
}

TEST_CASE("ByteBuffer bool read/write") {
    ByteBuffer buf;

    buf.write_bool(true);
    buf.write_bool(false);

    buf.reset_read();

    REQUIRE(buf.read_bool() == true);
    REQUIRE(buf.read_bool() == false);
    std::cout << "✅ 'ByteBuffer bool read/write\n";
}

TEST_CASE("ByteBuffer float/double BE/LE read/write") {
    ByteBuffer buf;

    float fval = 3.1415926f;
    double dval = 3.141592653589793;

    buf.write_float_be(fval);
    buf.write_float_le(fval);
    buf.write_double_be(dval);
    buf.write_double_le(dval);

    buf.reset_read();

    REQUIRE(buf.read_float_be() == Catch::Approx(fval).epsilon(0.0001));
    REQUIRE(buf.read_float_le() == Catch::Approx(fval).epsilon(0.0001));
    REQUIRE(buf.read_double_be() == Catch::Approx(dval).epsilon(0.0000001));
    REQUIRE(buf.read_double_le() == Catch::Approx(dval).epsilon(0.0000001));
    std::cout << "✅ 'ByteBuffer float/double BE/LE read/write\n";
}

TEST_CASE("ByteBuffer string raw BE/LE read/write") {
    ByteBuffer buf;
    std::string s = "ABCD";

    buf.write_string_raw_be(s);
    buf.write_string_raw_le(s);

    buf.reset_read();

    REQUIRE(buf.read_string_raw_be(s.size()) == s);
    REQUIRE(buf.read_string_raw_le(s.size()) == s);
    std::cout << "✅ 'ByteBuffer string raw BE/LE read/write\n";
}

TEST_CASE("ByteBuffer string NT BE/LE read/write") {
    ByteBuffer buf;
    std::string s = "TestString";

    buf.write_string_nt_be(s);
    buf.write_string_nt_le(s);

    buf.reset_read();

    REQUIRE(buf.read_string_nt_be() == s);
    REQUIRE(buf.read_string_nt_le() == s);
    std::cout << "✅ 'ByteBuffer string NT BE/LE read/write\n";
}

TEST_CASE("ByteBuffer skip & bytes") {
    ByteBuffer buf;

    buf.write_uint8(0x11);
    buf.write_uint8(0x22);
    buf.write_uint8(0x33);
    buf.write_uint8(0x44);

    buf.reset_read();

    REQUIRE(buf.read_uint8() == 0x11);
    buf.skip(2);
    REQUIRE(buf.read_uint8() == 0x44);
    std::cout << "✅ 'ByteBuffer skip & bytes\n";
}

TEST_CASE("ByteBuffer read_bytes") {
    ByteBuffer buf;
    buf.write_uint8(0xAA);
    buf.write_uint8(0xBB);
    buf.write_uint8(0xCC);

    buf.reset_read();

    auto bytes = buf.read_bytes(3);
    REQUIRE(bytes.size() == 3);
    REQUIRE(bytes[0] == 0xAA);
    REQUIRE(bytes[1] == 0xBB);
    REQUIRE(bytes[2] == 0xCC);
    std::cout << "✅ 'ByteBuffer read_bytes\n";
}
