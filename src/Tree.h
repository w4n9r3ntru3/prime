// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <memory>

#include "Node.h"
#include "safe.h"

template <typename T>
class Tree {
   public:
    // constructor
    Tree() noexcept
        : nodes(std::move(safe::vector<TreeNode>())),
          data(std::move(safe::vector<T>())) {}
    explicit Tree(size_t size) noexcept
        : nodes(std::move(safe::vector<TreeNode>(size))),
          data(std::move(safe::vector<T>(size))) {}
    explicit Tree(safe::vector<T>&& data_list) noexcept
        : data(std::move(data_list)) {
        nodes = safe::vector<TreeNode>();
        nodes.reserve(data.size());
        for (unsigned i = 0; i < data.size(); ++i) {
            nodes.push_back(TreeNode((unsigned)i));
        }
    }
    Tree(Tree&& tn) noexcept
        : nodes(std::move(tn.nodes)), data(std::move(tn.data)) {}

    // operator=
    Tree& operator=(Tree&& tn) noexcept {
        nodes = std::move(tn.nodes);
        data = std::move(tn.data);

        return *this;
    }

    // getter
    const T& datum(size_t idx) const { return data[idx]; }
    T& datum(size_t idx) { return data[idx]; }

    const TreeNode& node(size_t idx) const { return nodes[idx]; }
    TreeNode& node(size_t idx) { return nodes[idx]; }

    bool has_self(size_t idx) const { return node(idx).has_self(); }
    bool has_parent(size_t idx) const { return node(idx).has_parent(); }
    bool has_left(size_t idx) const { return node(idx).has_left(); }
    bool has_right(size_t idx) const { return node(idx).has_right(); }

    const T& self_datum(size_t idx) const { return datum(node(idx).self()); }
    T& self_datum(size_t idx) { return datum(node(idx).self()); }
    const T& parent_datum(size_t idx) const {
        return datum(node(idx).parent());
    }
    T& parent_datum(size_t idx) { return datum(node(idx).parent()); }
    const T& left_datum(size_t idx) const { return datum(node(idx).left()); }
    T& left_datum(size_t idx) { return datum(node(idx).left()); }
    const T& right_datum(size_t idx) const { return datum(node(idx).right()); }
    T& right_datum(size_t idx) { return datum(node(idx).right()); }

    unsigned self_node(size_t idx) const { return node(idx).self(); }
    unsigned parent_node(size_t idx) const { return node(idx).parent(); }
    unsigned left_node(size_t idx) const { return node(idx).left(); }
    unsigned right_node(size_t idx) const { return node(idx).right(); }

    const T& operator[](size_t idx) const { return datum(idx); }
    T& operator[](size_t idx) { return datum(idx); }

    const T& at(size_t idx) const { return datum(idx); }
    T& at(size_t idx) { return datum(idx); }

    bool is_leaf(size_t idx) const {
        const TreeNode& n = node(idx);
        return (!n.has_left()) && (!n.has_right());
    }

    void push(T&& dat) {
        safe::assert(nodes.size() == data.size());
        const unsigned size = nodes.size();
        nodes.push_back(std::move(TreeNode(size)));
        data.push_back(std::move(dat));
    }

   private:
    // fields
    safe::vector<TreeNode> nodes;
    safe::vector<T> data;
};
