// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

namespace safe {

template <typename K, typename V>
class unordered_map {
   public:
    unordered_map(void) noexcept : field(std::unordered_map<K, V>()) {}
    unordered_map(const unordered_map& hashmap) noexcept
        : field(hashmap.field) {}
    unordered_map(unordered_map&& hashmap) noexcept
        : field(std::move(hashmap.field)) {}

    unordered_map& operator=(const unordered_map& hashmap) noexcept {
        field = hashmap.field;
        return *this;
    }
    unordered_map& operator=(unordered_map&& hashmap) noexcept {
        field = std::move(hashmap.field);
        return *this;
    }

    void reserve(size_t capacity) { field.reserve(capacity); }
    bool empty(void) const { return field.empty(); }
    void clear(void) { field.clear(); }

    void rehash(size_t new_size) { field.rehash(new_size); }
    size_t size(void) const { return field.size(); }

    const V& operator[](const K& key) const { return find(key)->second; }
    V& operator[](const K& key) { return field[key]; }

    const V& at(const K& key) const {
        auto iter = find(key);
        assert(iter != end());
        return iter->second;
    }
    V& at(const K& key) {
        assert(contains(key));
        return field[key];
    }

    size_t erase(const K& key) {
        assert(contains(key));
        return field.erase(key);
    }

    typename std::unordered_map<K, V>::iterator begin(void) {
        return field.begin();
    }
    typename std::unordered_map<K, V>::const_iterator begin(void) const {
        return field.begin();
    }

    typename std::unordered_map<K, V>::iterator end(void) {
        return field.end();
    }
    typename std::unordered_map<K, V>::const_iterator end(void) const {
        return field.end();
    }

    typename std::unordered_map<K, V>::iterator find(const K& key) {
        return field.find(key);
    }
    typename std::unordered_map<K, V>::const_iterator find(const K& key) const {
        return field.find(key);
    }

    bool contains(const K& key) const { return find(key) != end(); }

    friend std::ostream& operator<<(std::ostream& out,
                                    const unordered_map& um) {
        std::stringstream ss;
        ss << "{";

        auto iter = um.begin();
        if (um.size() != 0) {
            ss << iter->first << ":" << iter->second;
        }
        for (++iter; iter != um.end(); ++iter) {
            ss << ", " << iter->first << ":" << iter->second;
        }
        ss << "}";
        out << ss.str();
        return out;
    }

   private:
    std::unordered_map<K, V> field;
};
}  // namespace safe
