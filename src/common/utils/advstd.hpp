#pragma once

#include <version>

#include <cstring> // for memcpy
#include <compare>

// this namespace holds implementations of upcoming stdlib features that our c++ version doesn't have yet
namespace advstd {
// libc++ is missing these two
    [[nodiscard]] constexpr bool is_eq(std::partial_ordering cmp) noexcept { return cmp == 0; }

    [[nodiscard]] constexpr bool is_neq(std::partial_ordering cmp) noexcept { return cmp != 0; }

// libstdc++ v10 is missing this
    template<typename To, typename From,
            std::enable_if_t<std::conjunction_v<
                    std::bool_constant<sizeof(To) == sizeof(From)>,
                    std::is_trivially_copyable<To>,
                    std::is_trivially_copyable<From>>, int> = 0>
    [[nodiscard]] constexpr To bit_cast(From const &from) noexcept {
        To to;
        std::memcpy(&to, &from, sizeof(To));
        return to;
    }
}