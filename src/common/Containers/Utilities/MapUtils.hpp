#include <type_traits>

namespace Containers {
/**
 * Returns a pointer to mapped value (or the value itself if map stores pointers)
 */
    template<class M>
    auto MapGetValuePtr(M &map, typename M::key_type const &key) {
        auto itr = map.find(key);
        if constexpr (std::is_pointer_v<typename M::mapped_type>)
            return itr != map.end() ? itr->second : nullptr;
        else
            return itr != map.end() ? &itr->second : nullptr;
    }

    template<class K, class V, template<class, class, class...> class M, class... Rest>
    void MultimapErasePair(M<K, V, Rest...> &multimap, K const &key, V const &value) {
        auto range = multimap.equal_range(key);
        for (auto itr = range.first; itr != range.second;) {
            if (itr->second == value)
                itr = multimap.erase(itr);
            else
                ++itr;
        }
    }
}