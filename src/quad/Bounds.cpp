// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "Bounds.h"

#include <assert.h>

template <typename T>
inline T maximum(T a, T b) {
    return a < b ? b : a;
}

template <typename T>
inline T minimum(T a, T b) {
    return a > b ? b : a;
}

constexpr Bounds::Bounds(void) noexcept : t(0), b(0), l(0), r(0) {}

Bounds::Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept
    : t(t), b(b), l(l), r(r) {}

Bounds::Bounds(const Bounds& b) noexcept : t(b.t), b(b.b), l(b.l), r(b.r) {}

Bounds::Bounds(Bounds&& b) noexcept : t(b.t), b(b.b), l(b.l), r(b.r) {}

Bounds::Bounds(const Bounds& lft, const Bounds& rgt) noexcept {
    t = maximum(lft.t, rgt.t);
    b = minimum(lft.b, rgt.b);
    l = minimum(lft.l, rgt.l);
    r = maximum(lft.r, rgt.r);
}

inline Bounds& Bounds::operator=(const Bounds& o) noexcept {
    t = o.t;
    b = o.b;
    l = o.l;
    r = o.r;
    return *this;
}

inline bool Bounds::operator==(const Bounds& o) const noexcept {
    return t == o.t && b == o.b && l == o.l && r == o.r;
}

inline bool Bounds::operator!=(const Bounds& b) const noexcept {
    return !(*this == b);
}

inline bool Bounds::is_initialized(void) const {
    return *this != Bounds();
}

inline unsigned Bounds::top(void) const {
    return t;
}

inline unsigned Bounds::bottom(void) const {
    return b;
}

inline unsigned Bounds::left(void) const {
    return l;
}

inline unsigned Bounds::right(void) const {
    return r;
}

inline void Bounds::top(unsigned val) {
    t = (int)val;
}

inline void Bounds::bottom(unsigned val) {
    b = (int)val;
}

inline void Bounds::left(unsigned val) {
    l = (int)val;
}

inline void Bounds::right(unsigned val) {
    r = (int)val;
}

inline bool Bounds::in_box(unsigned x, unsigned y) const {
    int ix = x, iy = y;
    return ix <= t && ix >= b && iy <= r && iy >= l;
}

inline std::pair<Corner, Corner> Bounds::overlap_with(const Bounds& oth) const {
    return std::make_pair(contains_point(oth), oth.contains_point(*this));
}

inline Corner Bounds::contains_point(const Bounds& oth) const {
    if (in_box(oth.l, oth.b)) {
        return Corner::BL;
    }
    if (in_box(oth.l, oth.t)) {
        return Corner::TL;
    }
    if (in_box(oth.r, oth.b)) {
        return Corner::BR;
    }
    if (in_box(oth.r, oth.t)) {
        return Corner::TR;
    }

    return Corner::None;
}

inline std::pair<unsigned, unsigned> Bounds::corner(Corner cor) const {
    switch (cor) {
        case Corner::TL:
            return std::make_pair(left(), top());
        case Corner::BL:
            return std::make_pair(left(), bottom());
        case Corner::TR:
            return std::make_pair(right(), top());
        case Corner::BR:
            return std::make_pair(right(), bottom());
        case Corner::None:
            assert(0);
    }
    assert(0);
}

float Bounds::centx(void) const {
    return (float)(l + r) / 2.f;
}

float Bounds::centy(void) const {
    return (float)(t + b) / 2.f;
}

BoundsNode::BoundsNode(void) noexcept : d(Bounds()) {}

BoundsNode::BoundsNode(Bounds&& b) noexcept : d(std::move(b)) {}

BoundsNode::BoundsNode(Bounds&& b,
                       std::shared_ptr<BoundsNode> left,
                       std::shared_ptr<BoundsNode> right) noexcept
    : d(std::move(b)), l(left), r(right) {}

BoundsNode::BoundsNode(std::shared_ptr<BoundsNode> left,
                       std::shared_ptr<BoundsNode> right) noexcept
    : d(Bounds(left->data(), right->data())), l(left), r(right) {}

BoundsNode::BoundsNode(Bounds&& b,
                       std::shared_ptr<BoundsNode> left,
                       std::shared_ptr<BoundsNode> right,
                       std::weak_ptr<BoundsNode> parent) noexcept
    : d(std::move(b)), l(left), r(right), p(parent) {}

BoundsNode::BoundsNode(BoundsNode&& other) noexcept
    : d(std::move(other.d)),
      l(std::move(other.l)),
      r(std::move(other.r)),
      p(std::move(other.p)) {}

std::weak_ptr<BoundsNode> BoundsNode::left(void) {
    return l;
}

const std::weak_ptr<BoundsNode> BoundsNode::left(void) const {
    return l;
}

void BoundsNode::left(std::weak_ptr<BoundsNode> lft) {
    l = lft.lock();
}

std::weak_ptr<BoundsNode> BoundsNode::right(void) {
    return r;
}

const std::weak_ptr<BoundsNode> BoundsNode::right(void) const {
    return r;
}

void BoundsNode::right(std::weak_ptr<BoundsNode> rgt) {
    r = rgt.lock();
}

std::weak_ptr<BoundsNode> BoundsNode::parent(void) {
    return p;
}

const std::weak_ptr<BoundsNode> BoundsNode::parent(void) const {
    return p;
}

void BoundsNode::parent(std::weak_ptr<BoundsNode> par) {
    p = par;
}

Bounds BoundsNode::data(void) const {
    return d;
}

void BoundsNode::data(Bounds bnd) {
    d = bnd;
}

BoundsTree::BoundsTree(void) noexcept
    : root(std::shared_ptr<BoundsNode>(nullptr)), sz(0) {}

BoundsTree::BoundsTree(Bounds&& b) noexcept
    : root(std::make_shared<BoundsNode>(std::move(b))), sz(1) {}

BoundsTree::BoundsTree(BoundsNode&& b) noexcept
    : root(std::make_shared<BoundsNode>(std::move(b))), sz(1) {}

BoundsTree::BoundsTree(BoundsTree&& bt) noexcept
    : root(std::move(bt.root)), sz(bt.sz) {}

BoundsTree::BoundsTree(BoundsTree&& a, BoundsTree&& b) noexcept
    : root(std::make_shared<BoundsNode>(a.root, b.root)), sz(a.sz + b.sz + 1) {}

inline size_t BoundsTree::size(void) const {
    return sz;
}

void BoundsTree::flatten(safe::vector<Bounds>& bounds) const {
    if (root == nullptr) {
        assert(sz == 0);
        return;
    }

    bounds.push_back(root->data());
}

inline void BoundsTree::validate(void) const {
    size_check();
    bounds_check();
}

inline void BoundsTree::size_check(void) const {
    safe::vector<Bounds> b;
    flatten(b);
    assert(size() == b.size());
}

static void recursive_bounds_check(std::shared_ptr<BoundsNode> node) {
    if (node == nullptr) {
        return;
    }

    std::shared_ptr<BoundsNode> l = node->left().lock(),
                                r = node->right().lock();

    if (l == nullptr && r == nullptr) {
    } else if (l == nullptr) {
        assert(node->data() == r->data());
    } else if (r == nullptr) {
        assert(node->data() == l->data());
    } else {
        assert(node->data() == Bounds(l->data(), r->data()));
    }

    recursive_bounds_check(l);
    recursive_bounds_check(r);
}

inline void BoundsTree::bounds_check(void) const {
    recursive_bounds_check(root);
}

safe::list<EndPoints> BoundsNode::top_down(void) const {
    auto left = l, right = r;

    auto llist = (l == nullptr) ? safe::list<EndPoints>() : l->top_down(),
         rlist = (r == nullptr) ? safe::list<EndPoints>() : r->top_down();

    llist.splice(llist.end(), rlist);
    assert(rlist.empty());

    auto lbox = l->data(), rbox = r->data();

    auto contains = lbox.overlap_with(rbox);

    Corner lcontr = contains.first, rcontl = contains.second;

    EndPoints ep;

    if (lcontr == Corner::None && rcontl == Corner::None) {
        // no overlap
        bool tb = lbox.top() < rbox.bottom();
        bool bt = lbox.bottom() > rbox.top();
        bool lr = lbox.left() > rbox.right();
        bool rl = lbox.right() < rbox.left();

        if (tb || bt || lr || rl) {
            // not side by side
            if (tb) {
                if (lr) {
                    ep = EndPoints(lbox.corner(Corner::TL),
                                   rbox.corner(Corner::BR));
                } else {
                    assert(rl);
                    ep = EndPoints(lbox.corner(Corner::TR),
                                   rbox.corner(Corner::BL));
                }
            } else {
                assert(bt);
                if (lr) {
                    ep = EndPoints(lbox.corner(Corner::BL),
                                   rbox.corner(Corner::TR));
                } else {
                    assert(rl);
                    ep = EndPoints(lbox.corner(Corner::BR),
                                   rbox.corner(Corner::TL));
                }
            }
        } else {
            // side by side
            if (tb) {
                ep = EndPoints(std::make_pair(lbox.centx(), lbox.top()),
                               std::make_pair(rbox.centx(), rbox.bottom()));
            } else if (bt) {
                ep = EndPoints(std::make_pair(lbox.centx(), lbox.bottom()),
                               std::make_pair(rbox.centx(), rbox.top()));
            } else if (lr) {
                ep = EndPoints(std::make_pair(lbox.left(), lbox.centy()),
                               std::make_pair(rbox.right(), rbox.centy()));
            } else {
                assert(rl);
                ep = EndPoints(std::make_pair(lbox.right(), lbox.centy()),
                               std::make_pair(rbox.left(), rbox.centy()));
            }
        }
    } else if (lcontr == Corner::None) {
        auto middle = lbox.corner(rcontl);
        ep = std::make_pair(middle, middle);
    } else if (rcontl == Corner::None) {
        auto middle = rbox.corner(lcontr);
        ep = std::make_pair(middle, middle);
    } else {
        assert(lcontr != Corner::None && rcontl != Corner::None);
        switch (lcontr) {
            case Corner::TL:
                assert(rcontl == Corner::BR);
                ep =
                    EndPoints(lbox.corner(Corner::TL), rbox.corner(Corner::BR));
                break;
            case Corner::TR:
                assert(rcontl == Corner::BL);
                ep =
                    EndPoints(lbox.corner(Corner::TR), rbox.corner(Corner::BL));
                break;
            case Corner::BL:
                assert(rcontl == Corner::TR);
                ep =
                    EndPoints(lbox.corner(Corner::BL), rbox.corner(Corner::TR));
                break;
            case Corner::BR:
                assert(rcontl == Corner::TL);
                ep =
                    EndPoints(lbox.corner(Corner::BR), rbox.corner(Corner::TL));
                break;
            case Corner::None:
                assert(0);
                break;
        }
    }

    llist.push_front(ep);

    return llist;
}

safe::list<EndPoints> BoundsTree::top_down(void) const {
    return root->top_down();
}
