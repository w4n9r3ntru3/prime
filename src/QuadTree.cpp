// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadTree.h"

QuadTree::QuadTree() noexcept : _NetName(""), _NetId(-1), _minLayer(-1) {
    segments = new safe::vector<NetSegment>;
}

const std::string& QuadTree::get_name() const {
    return _NetName;
}
const int QuadTree::get_net_id() const {
    return _NetId;
}
const int QuadTree::get_min_layer() const {
    return _minLayer;
}
const int QuadTree::get_root_idx() const {
    return root_idx;
}

void QuadTree::add_pin(Pin* p) {
    // TODO:
}

void QuadTree::add_segment(int srow,
                           int scol,
                           int slay,
                           int erow,
                           int ecol,
                           int elay) {
    if (slay != elay)
        return;  // ignore via
    segments->push_back(NetSegment(srow, scol, erow, ecol));
}

void QuadTree::construct_tree() {
    // TODO:
}