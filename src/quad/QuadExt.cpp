// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

// TODO: a bunch of methods that are extensions to Quad* classes

#include <assert.h>

#include <functional>

#include "QuadTree.h"

typedef safe::vector<SimplePin>::const_iterator citer;

enum class CmpType { Minimum, Maximum };

template <CmpType t>
unsigned get_(const safe::vector<SimplePin>& pins,
              std::function<unsigned(citer)> get) {
    auto iter = pins.begin();
    unsigned val = get(iter);
    for (++iter; iter != pins.end(); ++iter) {
        unsigned cur = get(iter);
        switch (t) {
            case CmpType::Minimum:
                if (cur < val) {
                    val = cur;
                }
                break;
            case CmpType::Maximum:
                if (cur > val) {
                    val = cur;
                }
                break;
        }
    }
    return val;
}

unsigned QuadTree::get_left(void) const {
    auto func = [=](citer c) { return c->get_row(); };
    return get_<CmpType::Minimum>(pins, func);
}

unsigned QuadTree::get_right(void) const {
    auto func = [=](citer c) { return c->get_row(); };
    return get_<CmpType::Maximum>(pins, func);
}

unsigned QuadTree::get_bottom(void) const {
    auto func = [=](citer c) { return c->get_col(); };
    return get_<CmpType::Minimum>(pins, func);
}

unsigned QuadTree::get_top(void) const {
    auto func = [=](citer c) { return c->get_col(); };
    return get_<CmpType::Maximum>(pins, func);
}

std::pair<unsigned, unsigned> QuadTree::get_horiz_bound(void) const {
    unsigned left = get_left(), right = get_right();
    assert(left <= right);
    return std::make_pair(left, right);
}

std::pair<unsigned, unsigned> QuadTree::get_verti_bound(void) const {
    unsigned bottom = get_bottom(), top = get_top();
    assert(bottom <= top);
    return std::make_pair(bottom, top);
}
