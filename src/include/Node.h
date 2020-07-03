// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>

#include "safe.h"

// * UnionFind uses its parent pointer
// * Tree uses its left child and right child
class TreeNode {
   public:
    // constructor
    TreeNode() noexcept;
    TreeNode(unsigned idx) noexcept;
    TreeNode(unsigned self,
             unsigned parent,
             unsigned left,
             unsigned right) noexcept;
    explicit TreeNode(const TreeNode& tn) noexcept;
    explicit TreeNode(TreeNode&& tn) noexcept;

    // operator=
    TreeNode& operator=(const TreeNode& tn) noexcept;
    TreeNode& operator=(TreeNode&& tn) noexcept;

    bool has_self() const;
    bool has_parent() const;
    bool has_left() const;
    bool has_right() const;

    unsigned self() const;
    unsigned parent() const;
    unsigned left() const;
    unsigned right() const;

    void self(unsigned s);
    void parent(unsigned p);
    void left(unsigned l);
    void right(unsigned r);

   private:
    int slf, par, lft, rgt;

    // friends
    friend std::ostream& operator<<(std::ostream& out, const TreeNode& tn);
};
