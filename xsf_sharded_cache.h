#ifndef XSF_SHARDED_CACHE_H
#define XSF_SHARDED_CACHE_H

#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "xsf_cache.h"

namespace xsf_simple_cache {

template <typename K, typename V, typename Cache, typename Hash = std::hash<K>>
class XSFShardedCache : public XSFCache<K, V> {
    static_assert(std::is_base_of_v<XSFCache<K, V>, Cache>,
                  "Cache must inherit from XSFCache<K, V>");

   public:
    explicit XSFShardedCache(size_t capacity)
        : XSFShardedCache(
              capacity, std::max(size_t(1),
                                 size_t(std::thread::hardware_concurrency()))) {
    }
    XSFShardedCache(size_t capacity, size_t shard_num, Hash hash = Hash())
        : capacity_(capacity),
          shard_num_(std::max(size_t(1), shard_num)),
          hash_(std::move(hash)) {
        // 计算每个分片的容量（向上取整）
        size_t shard_capacity = (capacity_ + shard_num_ - 1) / shard_num_;
        shard_capacity = std::max(shard_capacity, size_t(1));

        // 创建分片
        shards_.reserve(shard_num_);
        for (size_t i = 0; i < shard_num_; ++i) {
            shards_.emplace_back(std::make_unique<Cache>(shard_capacity));
        }
    }

    void put(const K& key, const V& value) override {
        size_t shard_index = hash_(key) % shard_num_;
        shards_[shard_index]->put(key, value);
    }

    std::optional<V> get(const K& key) override {
        size_t shard_index = hash_(key) % shard_num_;
        return shards_[shard_index]->get(key);
    }

   private:
    const size_t capacity_;
    const size_t shard_num_;
    Hash hash_;
    std::vector<std::unique_ptr<Cache>> shards_;
};

}  // namespace xsf_simple_cache

#endif  // XSF_SHARDED_CACHE_H