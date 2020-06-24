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
    unordered_map() noexcept : field(std::unordered_map<K, V>()) {}
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
    bool empty() const { return field.empty(); }
    void clear() { field.clear(); }

    void rehash(size_t new_size) { field.rehash(new_size); }
    size_t size() const { return field.size(); }

    const V& operator[](const K& key) const { return field[key]; }
    V& operator[](const K& key) { return field[key]; }

    const V& at(const K& key) const {
        assert(contains(key));
        return field[key];
    }
    V& at(const K& key) {
        assert(contains(key));
        return field[key];
    }

    size_t erase(const K& key) {
        assert(contains(key));
        return field.erase(key);
    }

    typename std::unordered_map<K, V>::iterator begin() {
        return field.begin();
    }
    typename std::unordered_map<K, V>::const_iterator begin() const {
        return field.begin();
    }

    typename std::unordered_map<K, V>::iterator end() { return field.end(); }
    typename std::unordered_map<K, V>::const_iterator end() const {
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
