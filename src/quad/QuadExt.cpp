// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

// TODO: a bunch of methods that are extensions to Quad* classes

#include <assert.h>

#include <functional>

#include "QuadTree.h"

typedef safe::vector<SimplePin>::const_iterator citer;

enum class CmpType { Minimum, Maximum };
enum class UseArg { Yes, No };

// if arg then return arg(min,max) else return (min,max)
template <CmpType t, UseArg arg>
static unsigned get_(const safe::vector<SimplePin>& pins,
                     std::function<unsigned(citer)> get) {
    auto iter = pins.begin();
    unsigned val = get(iter);
    unsigned idx = 0, idx_val = 0;
    for (++iter, ++idx; iter != pins.end(); ++iter, ++idx) {
        unsigned cur = get(iter);
        switch (t) {
            case CmpType::Minimum:
                if (cur < val) {
                    val = cur;
                    idx_val = idx;
                }
                break;
            case CmpType::Maximum:
                if (cur > val) {
                    val = cur;
                    idx_val = idx;
                }
                break;
        }
    }
    switch (arg) {
        case UseArg::Yes:
            return idx_val;
        case UseArg::No:
            return val;
    }
}

template <UseArg arg>
static unsigned left(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_row(); };
    return get_<CmpType::Minimum, UseArg::No>(pins, func);
}

template <UseArg arg>
static unsigned right(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_row(); };
    return get_<CmpType::Maximum, UseArg::No>(pins, func);
}

template <UseArg arg>
static unsigned bottom(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_col(); };
    return get_<CmpType::Minimum, UseArg::No>(pins, func);
}

template <UseArg arg>
static unsigned top(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_col(); };
    return get_<CmpType::Maximum, UseArg::No>(pins, func);
}

unsigned QuadTree::get_left(void) const {
    return left<UseArg::No>(pins);
}

unsigned QuadTree::get_right(void) const {
    return right<UseArg::No>(pins);
}

unsigned QuadTree::get_bottom(void) const {
    return bottom<UseArg::No>(pins);
}

unsigned QuadTree::get_top(void) const {
    return top<UseArg::No>(pins);
}

unsigned QuadTree::get_left_pin(void) const {
    unsigned p = left<UseArg::Yes>(pins);
    return pins[p].get_idx();
}

unsigned QuadTree::get_right_pin(void) const {
    unsigned p = right<UseArg::Yes>(pins);
    return pins[p].get_idx();
}

unsigned QuadTree::get_bottom_pin(void) const {
    unsigned p = bottom<UseArg::Yes>(pins);
    return pins[p].get_idx();
}

unsigned QuadTree::get_top_pin(void) const {
    unsigned p = top<UseArg::Yes>(pins);
    return pins[p].get_idx();
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

unsigned QuadTree::get_hpwl(void) const {
    auto horiz = get_horiz_bound(), verti = get_verti_bound();
    return horiz.second - horiz.first + verti.second - verti.first;
}
