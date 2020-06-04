/*
 *    Copyright (C) 2020 Ren-Chu Wang, All Rights Reserved
 *    Unauthorized copying of this file, via any medium is strictly prohibited
 *    Proprietary and confidential
 */

#pragma once

#include <memory>
#include <utility>
#include <vector>

namespace safe {

template <class T>
class vector {
   public:
    vector() : field(std::vector<T>()) {}
    vector(vector& vec) : field(vec.field) {}
    vector(vector&& vec) : field(std::move(vec.field)) {}

    vector<T>& operator=(const vector<T>& other) {
        this->field = other->field;
        return *this;
    }
    vector<T>& operator=(vector<T>&& other) {
        this->field = std::move(other->field);
        return *this;
    }

    void reserve(unsigned long capacity) { this->field.reserve(capacity); }

    void resize(unsigned long new_size) { this->field.resize(new_size); }
    size_t size() const { return this->field.size(); }

    void push_back(T& value) { this->field.push_back(value); }
    void push_back(T&& value) { this->field.push_back(std::move(value)); }

    void pop_back() { return this->field.pop_back(); }

    auto begin() { return this->field.begin(); }
    auto begin() const { return this->field.begin(); }

    auto end() { return this->field.end(); }
    auto end() const { return this->field.end(); }

    const T& operator[](size_t index) const {
        assert(index < this->field.size());
        return this->field[index];
    }
    T& operator[](size_t index) {
        assert(index < this->field.size());
        return this->field[index];
    }

   private:
    std::vector<T> field;
};

}  // namespace safe
