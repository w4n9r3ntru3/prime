// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "Tree.h"

Tree::Tree() noexcept : nodes(std::move(safe::vector<TreeNode>())) {}

Tree::Tree(size_t size) noexcept
    : nodes(std::move(safe::vector<TreeNode>(size))) {}

Tree::Tree(Tree&& tn) noexcept : nodes(std::move(tn.nodes)) {}

// operator=
Tree& Tree::operator=(Tree&& tn) noexcept {
    nodes = std::move(tn.nodes);
    return *this;
}

// getter

const TreeNode& Tree::node(size_t idx) const {
    return nodes[idx];
}

TreeNode& Tree::node(size_t idx) {
    return nodes[idx];
}

bool Tree::has_self(size_t idx) const {
    return node(idx).has_self();
}

bool Tree::has_parent(size_t idx) const {
    return node(idx).has_parent();
}

bool Tree::has_left(size_t idx) const {
    return node(idx).has_left();
}

bool Tree::has_right(size_t idx) const {
    return node(idx).has_right();
}

unsigned Tree::node_parent(size_t idx) const {
    return node(idx).parent();
}

unsigned Tree::node_left(size_t idx) const {
    return node(idx).left();
}
unsigned Tree::node_right(size_t idx) const {
    return node(idx).right();
}

const TreeNode& Tree::operator[](size_t idx) const {
    return node(idx);
}

TreeNode& Tree::operator[](size_t idx) {
    return node(idx);
}

const TreeNode& Tree::at(size_t idx) const {
    return node(idx);
}

TreeNode& Tree::at(size_t idx) {
    return node(idx);
}

bool Tree::is_leaf(size_t idx) const {
    const TreeNode& n = node(idx);
    return (!n.has_left()) && (!n.has_right());
}

void Tree::push_back(TreeNode&& node) {
    nodes.push_back(std::move(node));
}

size_t Tree::size() const {
    return nodes.size();
}

std::ostream& operator<<(std::ostream& out, const Tree& tree) {
    out << tree.nodes << "\n";
    return out;
}
