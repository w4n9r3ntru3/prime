// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

// TODO handle merging tree

#pragma once

#include <utility>
#include "Node.h"
#include "Tree.h"
#include "safe.h"

class MergeTree {
   public:
    void merge(void) {}

    static void merge_tree(
        unsigned num_pins,
        safe::vector<TreeNode>& edges,
        const safe::vector<std::pair<unsigned, unsigned>>& pairs);

   private:
    safe::vector<TreeNode> edges;
};
