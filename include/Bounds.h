// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <memory>
#include <utility>

#include "safe.h"

// TODO: finish tree implementation

using EndPoints = std::pair<std::pair<float, float>, std::pair<float, float>>;

enum class Corner { TL, TR, BL, BR, None };

// A bound is an HPWL. As simple as that.
class Bounds {
   public:
    // an empty bound
    constexpr Bounds(void) noexcept;
    // create a bound
    Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept;
    Bounds(const Bounds& bounds) noexcept;
    // replace another bound
    Bounds(Bounds&& bounds) noexcept;

    // construct bounds based on child
    Bounds(const Bounds& a, const Bounds& b) noexcept;

    // operators
    Bounds& operator=(const Bounds& b) noexcept;
    bool operator==(const Bounds& b) const noexcept;
    bool operator!=(const Bounds& b) const noexcept;

    bool is_initialized(void) const;

    unsigned top(void) const;
    unsigned bottom(void) const;
    unsigned left(void) const;
    unsigned right(void) const;

    void top(unsigned val);
    void bottom(unsigned val);
    void left(unsigned val);
    void right(unsigned val);

    bool in_box(unsigned x, unsigned y) const;

    float centx(void) const;
    float centy(void) const;

    std::pair<Corner, Corner> overlap_with(const Bounds& other) const;

    Corner contains_point(const Bounds& other) const;
    std::pair<unsigned, unsigned> corner(Corner cor) const;

   private:
    int t, b, l, r;
};

class BoundsNode {
   public:
    BoundsNode(void) noexcept;
    BoundsNode(Bounds&& b) noexcept;
    BoundsNode(Bounds&& b,
               std::shared_ptr<BoundsNode> left,
               std::shared_ptr<BoundsNode> right) noexcept;
    // ! primary constructor
    BoundsNode(std::shared_ptr<BoundsNode> left,
               std::shared_ptr<BoundsNode> right) noexcept;
    BoundsNode(Bounds&& b,
               std::shared_ptr<BoundsNode> left,
               std::shared_ptr<BoundsNode> right,
               std::shared_ptr<BoundsNode> parent) noexcept;
    BoundsNode(BoundsNode&& other) noexcept;

    void left(std::shared_ptr<BoundsNode> lft);
    void right(std::shared_ptr<BoundsNode> rgt);
    void parent(std::shared_ptr<BoundsNode> par);

    std::shared_ptr<BoundsNode> left(void);
    const std::shared_ptr<BoundsNode> left(void) const;
    std::shared_ptr<BoundsNode> right(void);
    const std::shared_ptr<BoundsNode> right(void) const;

    std::shared_ptr<BoundsNode> parent(void);
    const std::shared_ptr<BoundsNode> parent(void) const;

    Bounds data(void) const;
    void data(Bounds b);

    safe::list<EndPoints> top_down(void) const;

   private:
    Bounds d;
    std::shared_ptr<BoundsNode> l, r;
    std::shared_ptr<BoundsNode> p;
};

class BoundsTree {
   public:
    BoundsTree(void) noexcept;
    BoundsTree(Bounds&& b) noexcept;
    BoundsTree(BoundsNode&& b) noexcept;
    BoundsTree(BoundsTree&& bt) noexcept;
    BoundsTree(BoundsTree&& a, BoundsTree&& b) noexcept;

    size_t size(void) const;

    BoundsTree left(void) const;
    BoundsTree right(void) const;

    void flatten(safe::vector<Bounds>& bounds) const;

    void validate(void) const;

    safe::list<EndPoints> top_down(void) const;

   private:
    void size_check(void) const;
    void bounds_check(void) const;

    std::shared_ptr<BoundsNode> root;
    size_t sz;
};
