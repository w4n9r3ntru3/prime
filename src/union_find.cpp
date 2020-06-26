// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "union_find.h"

#include <assert.h>

#include <utility>

// * These functions can be used in merging tree constructors

bool group_by_rank(safe::vector<TreeNode>& treenodes,
                   safe::vector<unsigned>& ranks,
                   unsigned first_group,
                   unsigned second_group) {
    unsigned first_rank = ranks[first_group], second_rank = ranks[second_group];
    if (first_group == second_group) {
        return false;
    }
    if (first_rank < second_rank) {
        treenodes[first_group].parent(second_group);
    } else {
        treenodes[second_group].parent(first_group);
        if (first_rank == second_rank) {
            ++(ranks[first_group]);
        }
    }
    return true;
}

bool union_find_once(safe::vector<TreeNode>& treenodes,
                     safe::vector<unsigned>& ranks,
                     unsigned first,
                     unsigned second) {
    unsigned first_group = find<true>(first, treenodes),
             second_group = find<true>(second, treenodes);
    return group_by_rank(treenodes, ranks, first_group, second_group);
}

void union_find(safe::vector<TreeNode>& treenodes,
                const safe::vector<std::pair<unsigned, unsigned>>& pairs) {
    auto ranks = safe::vector<unsigned>(treenodes.size());
    std::fill(ranks.begin(), ranks.end(), 0);

    // number of times the loop does not terminate early
    unsigned compared = 0;

    for (auto pr : pairs) {
        unsigned first = pr.first, second = pr.second;

        if (union_find_once(treenodes, ranks, first, second)) {
            ++compared;
        }
    }

    assert(compared == treenodes.size() - 1);
}
