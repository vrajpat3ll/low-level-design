#ifndef EVICT_HPP_
#define EVICT_HPP_

#include <iostream>
#include <unordered_map>

#include "base.hpp"

template <typename Key_t>
class LRUAccessNode {
   public:
    Key_t key;
    LRUAccessNode* next;
    LRUAccessNode* prev;

    LRUAccessNode() : key{}, next(nullptr), prev(nullptr) {}
    LRUAccessNode(Key_t val) : key(val), next(nullptr), prev(nullptr) {}
};

template <typename Key_t>
class LRUEvictionPolicy : public BaseEvictionPolicy<Key_t> {
    std::unordered_map<Key_t, LRUAccessNode<Key_t>*> key2node;
    LRUAccessNode<Key_t>* head = nullptr;
    LRUAccessNode<Key_t>* tail = nullptr;

   public:
    LRUEvictionPolicy() {
        head = new LRUAccessNode<Key_t>;
        tail = new LRUAccessNode<Key_t>;

        head->next = tail;
        tail->prev = head;
    }
    void on_remove(const Key_t& key) {
        try {
            auto node = this->key2node.at(key);

            auto prev = node->prev;
            auto next = node->next;
            prev->next = next;
            next->prev = prev;

            delete node;
        } catch (...) {
            std::cout << "[on_remove] Key not found bro. Not inserted before!" << std::endl;
        }
    }
    void on_add(const Key_t& key) {
        auto to_add = new LRUAccessNode<Key_t>(key);

        auto next = head->next;
        head->next = to_add;
        to_add->prev = head;
        to_add->next = next;
        this->key2node[key] = to_add;
    }
    void on_access(const Key_t& key) {
        try {
            auto node = this->key2node.at(key);

            auto prev = node->prev;
            auto next = node->next;
            prev->next = next;
            next->prev = prev;
            auto hnext = head->next;
            head->next = node;
            hnext->prev = node;
        } catch (...) {
            std::cout << "[on_access] Key not found bro. Not inserted before!" << std::endl;
        }
    }
    Key_t evict_candidate() {
        return head->next->key;
    }

    ~LRUEvictionPolicy() {
        auto temp = head;
        while (temp != nullptr) {
            auto tnext = temp->next;
            delete temp;
            temp = tnext;
        }
    };
};
#endif  // EVICT_HPP_