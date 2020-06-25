// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>

namespace safe {

template <typename T>
class set {
   public:
    set(void) noexcept : field(std::set<T>()) {}
    set(const set& set) noexcept : field(set.field) {}
    set(set&& set) noexcept : field(std::move(set.field)) {}

    set& operator=(const set& set) noexcept {
        field = set.field;
        return *this;
    }
    set& operator=(set&& set) noexcept {
        field = std::move(set.field);
        return *this;
    }

    // * wrapper functions
    void clear(void) { field.clear(); }
    bool empty(void) const { return field.empty(); }
    std::pair<typename std::set<T>::iterator, bool> insert(const T& val) {
        return field.insert(std::move(val));
    }
    std::pair<typename std::set<T>::iterator, bool> insert(T&& val) {
        return field.insert(std::move(val));
    }

    typename std::set<T>::iterator begin(void) { return field.begin(); }
    typename std::set<T>::const_iterator begin(void) const {
        return field.begin();
    }

    typename std::set<T>::iterator end(void) { return field.end(); }
    typename std::set<T>::const_iterator end(void) const { return field.end(); }

    typename std::set<T>::iterator find(const T& element) {
        return field.find(element);
    }
    typename std::set<T>::const_iterator find(const T& element) const {
        return field.find(element);
    }

    bool contains(const T& element) const { return find(element) != end(); }

    friend std::ostream& operator<<(std::ostream& out, const set& set) {
        std::stringstream ss;

        auto iter = set.begin();
        ss << "{";
        if (set.size() != 0) {
            ss << *iter;
        }
        for (++iter; iter != set.end(); ++iter) {
            ss << ", " << *iter;
        }
        ss << "}";
        out << ss.str();
        return out;
    }

   private:
    std::set<T> field;
};

}  // namespace safe
