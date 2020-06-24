// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "Node.h"

TreeNode::TreeNode() noexcept : slf(-1), par(-1), lft(-1), rgt(-1) {}

TreeNode::TreeNode(unsigned idx) noexcept
    : slf((int)idx), par((int)idx), lft(-1), rgt(-1) {}

TreeNode::TreeNode(unsigned self,
                   unsigned parent,
                   unsigned left,
                   unsigned right) noexcept
    : slf((int)self), par((int)parent), lft((int)left), rgt((int)right) {}

TreeNode::TreeNode(const TreeNode& tn) noexcept {
    slf = tn.slf;
    par = tn.par;
    lft = tn.lft;
    rgt = tn.rgt;
}

TreeNode::TreeNode(TreeNode&& tn) noexcept {
    slf = tn.slf;
    par = tn.par;
    lft = tn.lft;
    rgt = tn.rgt;

    tn.slf = 0;
    tn.par = 0;
    tn.lft = 0;
    tn.rgt = 0;
}

TreeNode& TreeNode::operator=(const TreeNode& tn) noexcept {
    slf = tn.slf;
    par = tn.par;
    lft = tn.lft;
    rgt = tn.rgt;

    return *this;
}

TreeNode& TreeNode::operator=(TreeNode&& tn) noexcept {
    slf = tn.slf;
    par = tn.par;
    lft = tn.lft;
    rgt = tn.rgt;

    tn.slf = 0;
    tn.par = 0;
    tn.lft = 0;
    tn.rgt = 0;

    return *this;
}

bool TreeNode::has_self() const {
    return slf >= 0;
}
bool TreeNode::has_parent() const {
    return par >= 0;
}
bool TreeNode::has_left() const {
    return lft >= 0;
}
bool TreeNode::has_right() const {
    return rgt >= 0;
}

unsigned TreeNode::self() const {
    safe::assert(has_self());
    return (unsigned)slf;
}
unsigned TreeNode::parent() const {
    safe::assert(has_parent());
    return (unsigned)par;
}
unsigned TreeNode::left() const {
    safe::assert(has_left());
    return (unsigned)lft;
}
unsigned TreeNode::right() const {
    safe::assert(has_right());
    return (unsigned)rgt;
}

void TreeNode::self(unsigned s) {
    slf = (int)s;
}
void TreeNode::parent(unsigned p) {
    par = (int)p;
}
void TreeNode::left(unsigned l) {
    lft = (int)l;
}
void TreeNode::right(unsigned r) {
    rgt = (int)r;
}

std::ostream& operator<<(std::ostream& out, const TreeNode& tn) {
    out << "TreeNode{s:" << tn.slf << ", p:" << tn.par << ", l:" << tn.lft
        << ", r:" << tn.rgt << "}";

    return out;
}
