#ifndef STORAGE_HPP_
#define STORAGE_HPP_

#include <unordered_map>

#include "base.hpp"

template <typename Key_t, typename Value_t>
class Storage : public BaseStorage<Key_t, Value_t> {
    std::unordered_map<Key_t, Value_t> m_store;

   public:
    using key_type = Key_t;
    using value_type = Value_t;
    Storage() : m_store() {}

    std::optional<Value_t> get(const Key_t& key) const {
        try {
            return m_store.at(key);
        } catch (const std::exception& e) {
            std::cerr << "[Storage.get] " << e.what() << '\n';
            return std::nullopt;
        }
        return std::nullopt;
    }
    void put(const Key_t& key, const Value_t& value) {
        this->m_store[key] = value;
    }
    bool remove(const Key_t& key) {
        return this->m_store.erase(key) > 0;
    }
    size_t size() const { return this->m_store.size(); }

    virtual ~Storage() = default;
};
#endif  // STORAGE_HPP_