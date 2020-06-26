// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadNode.h"

#include <assert.h>

QuadNode::QuadNode() noexcept
    : self(-1), parent(-1), left(-1), right(-1), up(-1), down(-1){};

QuadNode::QuadNode(unsigned idx) noexcept
    : self((int)idx), parent((int)idx), left(-1), right(-1), up(-1), down(-1){};

QuadNode::QuadNode(unsigned self,
                   unsigned parent,
                   unsigned left,
                   unsigned right,
                   unsigned up,
                   unsigned down) noexcept
    : self((int)self),
      parent((int)parent),
      left((int)left),
      right((int)right),
      up((int)up),
      down((int)down){};

QuadNode::QuadNode(const QuadNode& qn) noexcept {
    self = qn.self;
    parent = qn.parent;
    left = qn.left;
    right = qn.right;
    up = qn.up;
    down = qn.down;
}

QuadNode::QuadNode(QuadNode&& qn) noexcept {
    self = qn.self;
    qn.self = 0;
    parent = qn.parent;
    qn.parent = 0;
    left = qn.left;
    qn.left = 0;
    right = qn.right;
    qn.right = 0;
    up = qn.up;
    qn.up = 0;
    down = qn.down;
    qn.down = 0;
}

bool QuadNode::has_self() const {
    return self >= 0;
}
bool QuadNode::has_parent() const {
    return parent >= 0;
}
bool QuadNode::has_left() const {
    return left >= 0;
}
bool QuadNode::has_right() const {
    return right >= 0;
}
bool QuadNode::has_up() const {
    return up >= 0;
}
bool QuadNode::has_down() const {
    return down >= 0;
}

unsigned QuadNode::get_self() const {
    assert(has_self());
    return self;
}
unsigned QuadNode::get_parent() const {
    assert(has_parent());
    return parent;
}
unsigned QuadNode::get_left() const {
    assert(has_left());
    return left;
}
unsigned QuadNode::get_right() const {
    assert(has_right());
    return right;
}
unsigned QuadNode::get_up() const {
    assert(has_up());
    return up;
}
unsigned QuadNode::get_down() const {
    assert(has_down());
    return down;
}

void QuadNode::set_self(unsigned s) {
    self = (int)s;
}
void QuadNode::set_parent(unsigned p) {
    parent = (int)p;
}
void QuadNode::set_left(unsigned l) {
    left = (int)l;
}
void QuadNode::set_right(unsigned r) {
    right = (int)r;
}
void QuadNode::set_up(unsigned u) {
    up = (int)u;
}
void QuadNode::set_down(unsigned d) {
    down = (int)d;
}

std::ostream& operator<<(std::ostream& out, const QuadNode& qn) {
    out << "QuadNode{s:" << qn.self << ", p:" << qn.parent << ", l:" << qn.left
        << ", r:" << qn.right << ", u:" << qn.up << ", d:" << qn.down << "}";

    return out;
}
