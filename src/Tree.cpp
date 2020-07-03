// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "include/Tree.h"

Tree::Tree(void) noexcept : nodes(std::move(safe::vector<TreeNode>())) {}

Tree::Tree(size_t size) noexcept
    : nodes(std::move(safe::vector<TreeNode>(size))) {}

Tree::Tree(safe::vector<TreeNode>&& nodes) noexcept : nodes(std::move(nodes)) {}

Tree::Tree(Tree&& tn) noexcept : nodes(std::move(tn.nodes)) {}

// operator=
Tree& Tree::operator=(Tree&& tn) noexcept {
    nodes = std::move(tn.nodes);
    return *this;
}

// getter

const TreeNode& Tree::get(size_t idx) const {
    return nodes[idx];
}

TreeNode& Tree::get(size_t idx) {
    return nodes[idx];
}

bool Tree::has_self(size_t idx) const {
    return get(idx).has_self();
}

bool Tree::has_parent(size_t idx) const {
    return get(idx).has_parent();
}

bool Tree::has_left(size_t idx) const {
    return get(idx).has_left();
}

bool Tree::has_right(size_t idx) const {
    return get(idx).has_right();
}

unsigned Tree::parent(size_t idx) const {
    return get(idx).parent();
}

unsigned Tree::left(size_t idx) const {
    return get(idx).left();
}
unsigned Tree::right(size_t idx) const {
    return get(idx).right();
}

const TreeNode& Tree::operator[](size_t idx) const {
    return get(idx);
}

TreeNode& Tree::operator[](size_t idx) {
    return get(idx);
}

const TreeNode& Tree::at(size_t idx) const {
    return get(idx);
}

TreeNode& Tree::at(size_t idx) {
    return get(idx);
}

bool Tree::is_leaf(size_t idx) const {
    const TreeNode& n = get(idx);
    return (!n.has_left()) && (!n.has_right());
}

void Tree::push_back(TreeNode&& node) {
    nodes.push_back(std::move(node));
}

size_t Tree::size(void) const {
    return nodes.size();
}

const safe::vector<TreeNode>& Tree::field(void) const {
    return nodes;
}
safe::vector<TreeNode>& Tree::field(void) {
    return nodes;
}

std::ostream& operator<<(std::ostream& out, const Tree& tree) {
    out << tree.nodes << "\n";
    return out;
}
