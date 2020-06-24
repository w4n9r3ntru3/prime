// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <iostream>
#include <utility>

#include "Node.h"
#include "safe.h"

// * These functions can be used in merging tree constructors

// * orphan means that parent can be illegal values (such as -1)
// * normally to indicate a root node, node.parent == node.self

template <bool orphan = false>
bool is_root(const TreeNode& node) {
    if (orphan) {
        return node.parent() < 0;
    } else {
        return node.self() == node.parent();
    }
}

template <bool orphan = false>
unsigned find(unsigned idx, safe::vector<TreeNode>& tree) {
    TreeNode& node = tree[idx];
    assert(node.has_self());

    // root node
    bool isroot = is_root<orphan>(node);

    // returns the root
    if (isroot) {
        return node.self();
    } else {
        // recursively calls parent
        unsigned root = find<orphan>(node.parent(), tree);
        node.parent(root);
        return root;
    }
}

template <bool orphan = false>
void run_union_find(safe::vector<TreeNode>& treenodes,
                    const safe::vector<std::pair<unsigned, unsigned>>& pairs) {
    auto ranks = safe::vector<unsigned>(treenodes.size());
    std::fill(ranks.begin(), ranks.end(), 0);

    // number of times the loop does not terminate early
    unsigned compared = 0;

    for (auto pr : pairs) {
        unsigned first = pr.first, second = pr.second;
        unsigned first_group = find(first, treenodes),
                 second_group = find(second, treenodes);
        unsigned first_rank = ranks[first_group],
                 second_rank = ranks[second_group];
        if (first_group == second_group) {
            continue;
        }
        if (first_rank < second_rank) {
            treenodes[first_group].parent(second_group);
        } else {
            treenodes[second_group].parent(first_group);
            if (first_rank == second_rank) {
                ++(ranks[first_group]);
            }
        }
        ++compared;
    }

    assert(compared == treenodes.size() - 1);
}
