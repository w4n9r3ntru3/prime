// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

// * This file contains STL containers that are already safe
// * Defined for consistency

#include <list>
#include <queue>

namespace safe {

template <typename... Args>
using priority_queue = std::priority_queue<Args...>;

template <typename... Args>
using list = std::list<Args...>;
}  // namespace safe
