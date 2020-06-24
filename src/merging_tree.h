// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

// TODO handle union find and merging tree
// Merging Tree Node

#pragma once

#include "Node.h"
#include "Tree.h"
#include "safe.h"

template <typename T>
class MergingTree {
   public:
    UnionFind(size_t size) noexcept : tree(Tree<T>(size)) {}

   private:
    Tree<T> tree;
};
