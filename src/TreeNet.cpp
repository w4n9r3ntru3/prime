// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "TreeNet.h"

#include <memory>

Point::Point(unsigned x, unsigned y, unsigned z) : i(x), j(y), k(z) {}

unsigned Point::x() const {
    return i;
}

unsigned Point::y() const {
    return j;
}

unsigned Point::z() const {
    return k;
}

void Point::x(unsigned val) {
    i = val;
}

void Point::y(unsigned val) {
    j = val;
}

void Point::z(unsigned val) {
    k = val;
}

Segment::Segment(unsigned s_x,
                 unsigned s_y,
                 unsigned s_z,
                 unsigned t_x,
                 unsigned t_y,
                 unsigned t_z)
    : src(std::move(Point(s_x, s_y, s_z))),
      tgt(std::move(Point(t_x, t_y, t_z))) {}

Segment::Segment(Point&& p1, Point&& p2)
    : src(std::move(p1)), tgt(std::move(p2)) {}

const Point& Segment::source() const {
    return src;
}

Point& Segment::source() {
    return src;
}

const Point& Segment::target() const {
    return tgt;
}

Point& Segment::target() {
    return tgt;
}

void Segment::source(Point&& point) {
    src = std::move(point);
}

void Segment::target(Point&& point) {
    tgt = std::move(point);
}

TreeNet::TreeNet(std::string&& name,
                 unsigned id,
                 unsigned num_pins,
                 unsigned min_layer)
    : net_name(std::move(name)), net_id(id), min_layer(min_layer) {
    connected.reserve(num_pins);
}

TreeNet::TreeNet(TreeNet&& tn)
    : net_name(std::move(tn.net_name)),
      net_id(tn.net_id),
      min_layer(tn.min_layer),
      connected(std::move(tn.connected)),
      tree_root(tn.tree_root),
      tree_size(tn.tree_size) {
    if (size()) {
        assert(tree_root != nullptr);
    }
    tn.tree_root = nullptr;
}

TreeNet::~TreeNet() {
    delete tree_root;
}

TreeNet& TreeNet::operator=(TreeNet&& tn) {
    net_name = std::move(tn.net_name);
    net_id = tn.net_id;
    min_layer = tn.min_layer;

    connected = std::move(tn.connected);

    tree_root = tn.tree_root;
    tree_size = tn.tree_size;

    tn.tree_root = nullptr;

    return *this;
}

void TreeNet::add_pin(Pin&& pin) {
    assert(connected.size() < connected.capacity());
    connected.push_back(std::move(pin));
}
