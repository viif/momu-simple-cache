#ifndef XSF_CACHE_H
#define XSF_CACHE_H

#include <optional>

namespace xsf_simple_cache {

template <typename K, typename V>
class XSFCache {
   public:
    virtual ~XSFCache() = default;

    virtual void put(const K& key, const V& value) = 0;
    virtual std::optional<V> get(const K& key) = 0;
};

}  // namespace xsf_simple_cache

#endif  // XSF_CACHE_H