// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include "Node.h"
#include "safe.h"

// * These functions can be used in merging tree constructors

template <bool compression>
unsigned find(unsigned idx, safe::vector<TreeNode>& tree) {
    TreeNode& node = tree[idx];
    assert(node.has_self());

    // returns the root
    if (!(node.has_parent())) {
        return node.self();
    } else {
        // recursively calls parent
        unsigned root = find(node.parent(), tree);
        if (compression) {
            node.parent(root);
        }
        return root;
    }
}
bool group_by_rank(safe::vector<TreeNode>& treenodes,
                   safe::vector<unsigned>& ranks,
                   unsigned first_group,
                   unsigned second_group);

bool union_find_once(safe::vector<TreeNode>& treenodes,
                     safe::vector<unsigned>& ranks,
                     unsigned first,
                     unsigned second);
void union_find(safe::vector<TreeNode>& treenodes,
                const safe::vector<std::pair<unsigned, unsigned>>& pairs);
