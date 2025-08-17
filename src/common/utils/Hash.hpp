#pragma once

#include <functional>
#include <string_view>
#include <utility>

template<typename T>
inline void hash_combine(std::size_t &seed, T const &val) {
    seed ^= std::hash<T>()(val) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
}

inline std::uint32_t HashFnv1a(std::string_view data) {
    std::uint32_t hash = 0x811C9DC5u;
    for (char c: data) {
        hash ^= c;
        hash *= 0x1000193u;
    }
    return hash;
}

//! Hash implementation for std::pair to allow using pairs in unordered_set or as key for unordered_map
//! Individual types used in pair must be hashable by std::hash
namespace std {
    template<class K, class V>
    struct hash<std::pair<K, V>> {
    public:
        size_t operator()(std::pair<K, V> const &p) const {
            size_t hashVal = 0;
            hash_combine(hashVal, p.first);
            hash_combine(hashVal, p.second);
            return hashVal;
        }
    };
}