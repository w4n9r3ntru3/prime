/*
 *    Copyright (C) 2020 Ren-Chu Wang, All Rights Reserved
 *    Unauthorized copying of this file, via any medium is strictly prohibited
 *    Proprietary and confidential
 */

#pragma once

#include "safe_vector.h"

template <class C, class T>
bool contains(const C& container, const T& element) {
    return container.find(element) != container.end();
}
