// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "MergeTree.h"

#include <assert.h>

#include "union_find.h"

void MergeTree::merge_tree(
    unsigned num_pins,
    safe::vector<TreeNode>& mtree,
    const safe::vector<std::pair<unsigned, unsigned>>& pairs) {
    const unsigned num_mtree = 2 * num_pins - 1;

    safe::vector<TreeNode> pins;
    safe::vector<unsigned> ranks;
    assert(mtree.size() == 0);

    pins.reserve(num_pins);
    ranks.reserve(num_pins);
    mtree.reserve(num_mtree);
    for (unsigned i = 0; i < num_pins; ++i) {
        mtree.push_back(std::move(TreeNode(i)));
        pins.push_back(std::move(TreeNode(i)));
        ranks.push_back(0U);
    }

    assert(pins.size() == num_pins);
    assert(ranks.size() == num_pins);

    // num_mtree iterations
    // mtree[0:num_mtree] are external nodes
    // mtree[num-mtree:] are internal nodes
    unsigned compared = 0;
    for (auto pr : pairs) {
        unsigned first = pr.first, second = pr.second;
        unsigned first_root = find<true>(first, pins),
                 second_root = find<true>(second, pins);
        if (group_by_rank(pins, ranks, first_root, second_root)) {
            // self < 0 means that the node is a leaf
            // self > 0 means that the node is internal

            // finding the subtree the selected nodes belong to
            unsigned subroot_1 = find<false>(first, mtree),
                     subroot_2 = find<false>(second, mtree);
            unsigned current = num_pins + compared;
            mtree.push_back(TreeNode(current, -1, subroot_1, subroot_2));
            mtree[subroot_1].parent(current);
            mtree[subroot_2].parent(current);
            ++compared;
        }
    }
    assert(compared == num_pins - 1);

    assert(mtree.size() == num_mtree);
}
