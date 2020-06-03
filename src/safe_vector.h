/*
 *    Copyright (C) 2020 Ren-Chu Wang, All Rights Reserved
 *    Unauthorized copying of this file, via any medium is strictly prohibited
 *    Proprietary and confidential
 */

#include <memory>
#include <utility>
#include <vector>

#include "safe_result.h"

namespace safe {

template <class T>
class vector {
   public:
    vector() : _field(std::vector<T>()) {}
    vector(vector& vec) : _field(vec._field) {}
    vector(vector&& vec) : _field(std::move(vec._field)) {}

    vector<T>& operator=(const vector<T>& other) {
        this->_field = other->_field;
        return *this;
    }
    vector<T>& operator=(vector<T>&& other) {
        this->_field = std::move(other->_field);
        return *this;
    }

    void reserve(unsigned long capacity) { this->_field.reserve(capacity); }

    void resize(unsigned long new_size) { this->_field.resize(new_size); }

    void push_back(T& value) { this->_field.push_back(value); }
    void push_back(T&& value) { this->_field.push_back(std::move(value)); }

    const Result<const T> operator[](unsigned long index) const {
        if (index < this->_field.size()) {
            return safe::Ok<const T>(this->_field[index]);
        }
        return safe::Err<const T>();
    }
    Result<T> operator[](unsigned long index) {
        if (index < this->_field.size()) {
            return safe::Ok<T>(this->_field[index]);
        }
        return safe::Err<T>();
    }

   private:
    std::vector<T> _field;
};

}  // namespace safe
