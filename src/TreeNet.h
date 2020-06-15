// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include "Cell.h"
#include "safe.h"
#include "union_find.h"

// TODO
// ! not sure yet how to assign route to connect pins.
// ? connect the routes such that the points only go from pin to pin
// ? use the GridNet as a temporal holder

class Point {
   public:
    Point(unsigned x, unsigned y, unsigned z);

    // getters
    unsigned x() const;
    unsigned y() const;
    unsigned z() const;

    // setters
    void x(unsigned val);
    void y(unsigned val);
    void z(unsigned val);

   private:
    // fields
    unsigned i, j, k;
};

class Segment {
   public:
    // constructors
    Segment(unsigned s_x,
            unsigned s_y,
            unsigned s_z,
            unsigned t_x,
            unsigned t_y,
            unsigned t_z);
    Segment(Point&& p1, Point&& p2);

    // getters
    const Point& source() const;
    Point& source();
    const Point& target() const;
    Point& target();

    // setters
    void source(Point&& p);
    void target(Point&& p);

   private:
    // fields
    Point src, tgt;
};

class TreeNet {
   public:
    // constructor
    TreeNet(std::string&& name,
            unsigned id,
            unsigned num_pins,
            unsigned min_layer);
    TreeNet(TreeNet&& tn);
    // destructor
    ~TreeNet();

    // operator=
    TreeNet& operator=(TreeNet&& tn);

    void add_pin(Pin&& pin);
    // void add_segments(const safe::vector<Segment>& segments) {
    // TODO
    // haven't worked out the details.
    // ? Use union-find to merge the segments and initialize the tree?
    // }

    size_t size() const { return connected.size(); }

   private:
    std::string net_name;
    unsigned net_id;
    unsigned min_layer;

    safe::vector<Pin> connected;

    // TODO this is not the final type
    Node<Segment>* tree_root;
    unsigned tree_size;
};
