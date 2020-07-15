// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <memory>

#include "safe.h"

// TODO: finish tree implementation

// A bound is a HPWL. As simple as that.
class Bounds {
   public:
    // an empty bound
    explicit constexpr Bounds(void) noexcept;
    // create a bound
    explicit Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept;
    explicit Bounds(const Bounds& bounds) noexcept = delete;
    // replace another bound
    explicit Bounds(Bounds&& bounds) noexcept;

    // construct bounds based on child
    explicit Bounds(const Bounds& a, const Bounds& b) noexcept;

    // operators
    bool operator==(const Bounds& b) const noexcept;
    bool operator!=(const Bounds& b) const noexcept;

    bool is_initialized(void) const;

    unsigned get_top(void) const;
    unsigned get_bottom(void) const;
    unsigned get_left(void) const;
    unsigned get_right(void) const;

    void set_top(unsigned val);
    void set_bottom(unsigned val);
    void set_left(unsigned val);
    void set_right(unsigned val);

   private:
    int top, bottom, left, right;
};

class BoundsNode {
   public:
    explicit BoundsNode(void) noexcept;
    explicit BoundsNode(std::shared_ptr<BoundsNode> left,
                        std::shared_ptr<BoundsNode> right) noexcept;
    explicit BoundsNode(std::shared_ptr<BoundsNode> left,
                        std::shared_ptr<BoundsNode> right,
                        std::shared_ptr<BoundsNode> parent) noexcept;

    void set_parent(std::shared_ptr<BoundsNode> parent);

   private:
    std::shared_ptr<BoundsNode> l, r;
    std::weak_ptr<BoundsNode> p;
};

class BoundsTree {
   public:
    explicit BoundsTree(void) noexcept;
    explicit BoundsTree(Bounds&& b) noexcept;
    explicit BoundsTree(BoundsTree&& bt) noexcept;
    explicit BoundsTree(BoundsTree&& a, BoundsTree&& b) noexcept;

    size_t size(void) const;

    BoundsTree left(void) const;
    BoundsTree right(void) const;

    safe::vector<Bounds> flatten(void) const;

   private:
    std::shared_ptr<BoundsNode> root;
    size_t sz;
};
