// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <sstream>

namespace safe {

template <typename T>
class deque {
   public:
    // typedef
    typedef typename std::deque<T>::size_type size_type;
    typedef typename std::deque<T>::value_type value_type;
    typedef typename std::deque<T>::reference reference;
    typedef typename std::deque<T>::const_reference const_reference;
    typedef typename std::deque<T>::iterator iterator;
    typedef typename std::deque<T>::const_iterator const_iterator;

    // constructor
    deque(void) noexcept : field(std::deque<T>()) {}
    deque(const deque& vec) noexcept : field(vec.field) {}
    deque(deque&& vec) noexcept : field(std::move(vec.field)) {}
    deque(size_t size) noexcept : field(std::deque<T>(size)) {}
    deque(size_t size, const T& value) noexcept
        : field(std::deque<T>(size, value)) {}

    // operator=
    deque& operator=(const deque& vec) noexcept {
        field = vec.field;
        return *this;
    }
    deque& operator=(deque&& vec) noexcept {
        field = std::move(vec.field);
        return *this;
    }

    // * wrapper functions
    void reserve(size_type capacity) { field.reserve(capacity); }

    void resize(size_type new_size) { field.resize(new_size); }
    size_type size(void) const { return field.size(); }

    bool empty(void) const { return field.empty(); }
    void clear(void) { field.clear(); }

    void push_back(const T& value) { field.push_back(value); }
    void push_back(T&& value) { field.push_back(std::move(value)); }

    void push_front(const T& value) { field.push_front(value); }
    void push_front(T&& value) { field.push_front(std::move(value)); }

    void pop_back(void) { return field.pop_back(); }

    T& front(void) { return field.front(); }
    const T& front(void) const { return field.front(); }

    T& operator[](size_t index) {
        assert(index < field.size());
        return field[index];
    }
    const T& operator[](size_t index) const {
        assert(index < field.size());
        return field[index];
    }

    T& at(size_t index) { return (*this)[index]; }
    const T& at(size_t index) const { return (*this)[index]; }

    iterator begin(void) { return field.begin(); }
    const_iterator begin(void) const { return field.begin(); }

    iterator end(void) { return field.end(); }
    const_iterator end(void) const { return field.end(); }

    iterator find(const T& element) {
        return std::find(field.begin(), field.end(), element);
    }
    const_iterator find(const T& element) const {
        return std::find(field.begin(), field.end(), element);
    }

    bool contains(const T& element) const { return find(element) != end(); }

    friend std::ostream& operator<<(std::ostream& out, const deque& vec) {
        std::stringstream ss;
        ss << "[";
        if (vec.size() != 0) {
            ss << vec[0];
        }
        for (size_t i = 1; i < vec.size(); ++i) {
            ss << ", " << vec[i];
        }
        ss << "]";
        out << ss.str();
        return out;
    }

   private:
    std::deque<T> field;
};

}  // namespace safe
