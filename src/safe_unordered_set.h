// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_set>

namespace safe {

template <typename T>
class unordered_set {
   public:
    unordered_set() : field(std::unordered_set<T>()) {}
    unordered_set(const unordered_set& set) : field(set.field) {}
    unordered_set(unordered_set&& set) : field(std::move(set.field)) {}

    unordered_set& operator=(const unordered_set& set) {
        field = set.field;
        return *this;
    }
    unordered_set& operator=(unordered_set&& set) {
        field = std::move(set.field);
        return *this;
    }

    // * wrapper functions
    void clear() { field.clear(); }
    bool empty() const { return field.empty(); }
    std::pair<typename std::unordered_set<T>::iterator, bool> insert(
        const T& val) {
        return field.insert(std::move(val));
    }
    std::pair<typename std::unordered_set<T>::iterator, bool> insert(T&& val) {
        return field.insert(std::move(val));
    }

    typename std::unordered_set<T>::iterator begin() { return field.begin(); }
    typename std::unordered_set<T>::const_iterator begin() const {
        return field.begin();
    }

    typename std::unordered_set<T>::iterator end() { return field.end(); }
    typename std::unordered_set<T>::const_iterator end() const {
        return field.end();
    }

    typename std::unordered_set<T>::iterator find(const T& element) {
        return field.find(element);
    }
    typename std::unordered_set<T>::const_iterator find(
        const T& element) const {
        return field.find(element);
    }

    bool contains(const T& element) const { return find(element) != end(); }

    friend std::ostream& operator<<(std::ostream& out,
                                    const unordered_set& set) {
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
    std::unordered_set<T> field;
};

}  // namespace safe
