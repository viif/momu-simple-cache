#ifndef MOMU_SIMPLE_CACHE_CACHE_H
#define MOMU_SIMPLE_CACHE_CACHE_H

#include <memory>
#include <optional>

namespace momu {

namespace simple_cache {

template <typename K, typename V>
class Cache {
   public:
    virtual ~Cache() = default;

    virtual void put(const K& key, const V& value) = 0;
    virtual std::optional<V> get(const K& key) = 0;
};

template <typename K, typename V>
class CacheCreator {
   public:
    virtual ~CacheCreator() = default;

    virtual std::unique_ptr<Cache<K, V>> create(size_t capacity) const = 0;
};

}  // namespace simple_cache

}  // namespace momu

#endif  // MOMU_SIMPLE_CACHE_CACHE_H