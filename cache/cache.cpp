#ifndef CACHE_HPP_
#define CACHE_HPP_

#include "base.hpp"
#include "evict.cpp"
#include "storage.cpp"

template <typename Key_t, typename Value_t>
class LRUCache : public BaseCache<Key_t, Value_t> {
   public:
    LRUCache(
        std::unique_ptr<Storage<Key_t, Value_t>> store,
        std::unique_ptr<LRUEvictionPolicy<Key_t>> policy, uint32_t capacity)
        : BaseCache<Key_t, Value_t>(
              std::move(store),
              std::move(policy), capacity) {}
};
#endif  // CACHE_HPP_