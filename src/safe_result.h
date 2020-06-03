/*
 *    Copyright (C) 2020 Ren-Chu Wang, All Rights Reserved
 *    Unauthorized copying of this file, via any medium is strictly prohibited
 *    Proprietary and confidential
 */

#include <assert.h>

#include <utility>

namespace safe {

// TODO there are possible lifetime issues here

template <class T>
class Result {
   public:
    // Callable when okay is false
    Result() : _ok(false), _field(nullptr) {}
    Result(bool okay) : _ok(okay), _field(nullptr) { assert(!okay); }
    // callable when okay is either
    Result(bool okay, T& value) : _ok(okay), _field(&value) {}

    bool ok() const { return _ok; }
    const T& field() const {
        assert(_ok);
        assert(_field != nullptr);
        return *_field;
    }
    T& field() {
        assert(_ok);
        assert(_field != nullptr);
        return *_field;
    }

   private:
    bool _ok;
    T* _field;
};

template <class T>
Result<T> Ok(T& reference) {
    return Result<T>(true, reference);
}

template <class T>
Result<T> Err() {
    return Result<T>(false);
}

}  // namespace safe
