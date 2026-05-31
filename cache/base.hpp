#ifndef BASE_HPP_
#define BASE_HPP_

#include <cstdint>
#include <memory>
#include <optional>

template <typename Key_t, typename Value_t>
class BaseStorage {
   public:
    using key_type = Key_t;
    using value_type = Value_t;

    virtual std::optional<Value_t> get(const Key_t&) const = 0;
    virtual void put(const Key_t&, const Value_t&) = 0;
    virtual bool remove(const Key_t&) = 0;
    virtual size_t size() const = 0;

    virtual ~BaseStorage() = default;
};

template <typename Key_t>
class BaseEvictionPolicy {
   public:
    virtual void on_remove(const Key_t&) = 0;
    virtual void on_add(const Key_t&) = 0;
    virtual void on_access(const Key_t&) = 0;
    virtual Key_t evict_candidate() = 0;

    virtual ~BaseEvictionPolicy() = default;
};

/// @brief We're using `runtime polymorphism` for the storage and eviction
/// policy right now. Basically I read it off ChatGPT about using a unique_ptr
/// instead of templatizing the storage and policy.
template <typename Key_t, typename Value_t>
class BaseCache {
    uint32_t m_capacity;

    /// @brief Cons: vtable lookups are slower
    /// if speed is a concern, use templates for both of these
    std::unique_ptr<BaseStorage<Key_t, Value_t>> m_store;
    std::unique_ptr<BaseEvictionPolicy<Key_t>> m_policy;

   public:
    BaseCache(std::unique_ptr<BaseStorage<Key_t, Value_t>> store,
              std::unique_ptr<BaseEvictionPolicy<Key_t>> policy, uint32_t capacity) : m_store(std::move(store)), m_policy(std::move(policy)), m_capacity(capacity) {}

    virtual ~BaseCache() = default;

    uint32_t size() { return this->m_store->size(); }
    uint32_t capacity() { return this->m_capacity; }

    std::optional<Value_t> get(Key_t key) {
        try {
            auto value = this->m_store->get(key);
            if (value.has_value()) {
                this->m_policy->on_access(key);
                return value.value();
            }
        } catch (...) {
            return std::nullopt;
        }
        return std::nullopt;
    }

    void remove(Key_t key) {
        if (this->m_store->remove(key)) {
            this->m_policy->on_remove(key);
        }
    }

    void put(Key_t key, Value_t value) {
        if (this->size() == this->m_capacity) {
            auto key_to_remove = this->m_policy->evict_candidate();
            this->remove(key_to_remove);
        }
        this->m_policy->on_add(key);
        this->m_store->put(key, value);
    }
};

#endif  // BASE_HPP_