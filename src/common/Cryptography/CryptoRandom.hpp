#include <array>
#include <cstdint>
#include <cstddef>

namespace Crypto {
    void GetRandomBytes(uint8_t *buf, size_t len);

    template<typename Container>
    void GetRandomBytes(Container &c) {
        GetRandomBytes(std::data(c), std::size(c));
    }

    template<size_t S>
    std::array<uint8_t, S> GetRandomBytes() {
        std::array<uint8_t, S> arr;
        GetRandomBytes(arr);
        return arr;
    }
}