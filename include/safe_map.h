// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <iostream>
#include <map>
#include <memory>
#include <sstream>

namespace safe {

template <typename K, typename V>
class map {
   public:
    // typedef
    typedef typename std::map<K, V>::size_type size_type;
    typedef typename std::map<K, V>::key_type key_type;
    typedef typename std::map<K, V>::value_type value_type;
    typedef typename std::map<K, V>::iterator iterator;
    typedef typename std::map<K, V>::const_iterator const_iterator;

    // constructor
    map(void) noexcept : field(std::map<K, V>()) {}
    map(const map& hashmap) noexcept : field(hashmap.field) {}
    map(map&& hashmap) noexcept : field(std::move(hashmap.field)) {}

    // operator=
    map& operator=(const map& hashmap) noexcept {
        field = hashmap.field;
        return *this;
    }
    map& operator=(map&& hashmap) noexcept {
        field = std::move(hashmap.field);
        return *this;
    }

    void reserve(size_type capacity) { field.reserve(capacity); }
    bool empty(void) const { return field.empty(); }
    void clear(void) { field.clear(); }

    void rehash(size_type new_size) { field.rehash(new_size); }
    size_type size(void) const { return field.size(); }

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

    size_type erase(const key_type& key) {
        assert(contains(key));
        return field.erase(key);
    }

    iterator begin(void) { return field.begin(); }
    const_iterator begin(void) const { return field.begin(); }

    iterator end(void) { return field.end(); }
    const_iterator end(void) const { return field.end(); }

    iterator find(const K& key) { return field.find(key); }
    const_iterator find(const K& key) const { return field.find(key); }

    bool contains(const K& key) const { return find(key) != end(); }

    friend std::ostream& operator<<(std::ostream& out, const map& um) {
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
    std::map<K, V> field;
};

}  // namespace safe
