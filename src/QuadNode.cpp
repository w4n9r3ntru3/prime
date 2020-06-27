// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadNode.h"

#include <assert.h>

QuadNode::QuadNode() noexcept 
    : self(-1), parent(-1), left(-1), right(-1), up(-1), down(-1), 
      coord_x(-1), coord_y(-1) {};

QuadNode::QuadNode(int idx) noexcept 
    : self(idx), parent(-1), left(-1), right(-1), up(-1), down(-1), 
      coord_x(-1), coord_y(-1) {};

QuadNode::QuadNode(int _s, int _p,
                   int _l, int _r,
                   int _u, int _d,
                   int _x ,int _y) noexcept
    : self(_s), parent(_p), left(_l), right(_r), up(_u), down(_d), 
      coord_x(_x), coord_y(_y) {};

QuadNode::QuadNode(const QuadNode& qn) noexcept {
    self   = qn.self;
    parent = qn.parent;
    left   = qn.left;
    right  = qn.right;
    up     = qn.up;
    down   = qn.down;
}

QuadNode::QuadNode(QuadNode&& qn) noexcept {
    self   =   qn.self; qn.self   = 0;
    parent = qn.parent; qn.parent = 0;
    left   =   qn.left; qn.left   = 0;
    right  =  qn.right; qn.right  = 0;
    up     =     qn.up; qn.up     = 0;
    down   =   qn.down; qn.down   = 0;
}

const bool QuadNode::is_root()    const { return parent == -1; }
const bool QuadNode::has_self()   const { return self   >= 0; }
const bool QuadNode::has_parent() const { return parent >= 0; }
const bool QuadNode::has_left()   const { return left   >= 0; }
const bool QuadNode::has_right()  const { return right  >= 0; }
const bool QuadNode::has_up()     const { return up     >= 0; }
const bool QuadNode::has_down()   const { return down   >= 0; }

const int QuadNode::get_self()   const { assert(has_self());   return self;   }
const int QuadNode::get_parent() const { assert(has_parent()); return parent; }
const int QuadNode::get_left()   const { assert(has_left());   return left;   }
const int QuadNode::get_right()  const { assert(has_right());  return right;  }
const int QuadNode::get_up()     const { assert(has_up());     return up;     }
const int QuadNode::get_down()   const { assert(has_down());   return down;   }

void QuadNode::set_self   (int s) { self   = s; }
void QuadNode::set_parent (int p) { parent = p; }
void QuadNode::set_left   (int l) { left   = l; }
void QuadNode::set_right  (int r) { right  = r; }
void QuadNode::set_up     (int u) { up     = u; }
void QuadNode::set_down   (int d) { down   = d; }

void QuadNode::reset_node(int s, int p, int l, int r, int u, int d){
    self = s; parent = p; left = l; right = r; up = u; down = d;
}

void QuadNode::set_x      (int c_x) { coord_x = c_x; }
void QuadNode::set_y      (int c_y) { coord_y = c_y; }
void QuadNode::reset_coord(CoordPair c) { coord_x = c.first; coord_y = c.second; }
void QuadNode::reset_coord(int c_x, int c_y) { coord_x = c_x; coord_y = c_y; }

std::ostream& operator<<(std::ostream& out, const QuadNode& qn) {
    out << "QuadNode{s:" << qn.self << ", p:" << qn.parent
        <<        ", l:" << qn.left << ", r:" << qn.right
        <<        ", u:" << qn.up   << ", d:" << qn.down
        << "}";

    return out;
}

SimpleEdge::SimpleEdge() noexcept 
    : v1(0), v2(0), weight(0) {}
SimpleEdge::SimpleEdge(unsigned _v1, unsigned _v2, unsigned _w) noexcept 
    : v1(_v1), v2(_v2), weight(_w) {}

const bool SimpleEdge::operator<(const SimpleEdge& se) const {
    if(weight != se.weight) return weight < se.weight;
    else if(v1 != se.v1)    return v1 < se.v1;
    else                    return v2 < se.v2;
}

const unsigned SimpleEdge::get_v1()     const { return v1; }
const unsigned SimpleEdge::get_v2()     const { return v2; }
const unsigned SimpleEdge::get_weight() const { return weight; }

