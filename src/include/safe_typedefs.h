// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

// * This file contains STL containers that are already safe
// * Defined for consistency

#include <queue>

namespace safe {

template <typename... Args>
using priority_queue = std::priority_queue<Args...>;

}  // namespace safe
