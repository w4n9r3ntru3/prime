// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "../include/QuadNode.h"

#include <assert.h>

QuadNode::QuadNode() noexcept 
    : self(-1), up(-1), down(-1), left(-1), right(-1), 
      coord_x(-1), coord_y(-1),
      lay_s(-1), lay_u(-1), lay_d(-1), lay_l(-1), lay_r(-1) {};

QuadNode::QuadNode(int idx) noexcept 
    : self(idx), up(-1), down(-1), left(-1), right(-1), 
      coord_x(-1), coord_y(-1) {};

QuadNode::QuadNode(int _s,
                   int _u, int _d,
                   int _l, int _r,
                   int _x ,int _y,
                   int _ls,
                   int _lu, int _ld,
                   int _ll, int _lr) noexcept
    : self(_s), up(_u), down(_d), left(_l), right(_r), 
      coord_x(_x), coord_y(_y), 
      lay_s(_ls), lay_u(_lu), lay_d(_ld), lay_l(_ll), lay_r(_lr) {};

QuadNode::QuadNode(const QuadNode& qn) noexcept {
    reset(qn);
}

QuadNode::QuadNode(QuadNode&& qn) noexcept {
    self    = qn.self;
    up      = qn.up;
    down    = qn.down;
    left    = qn.left;
    right   = qn.right;
    coord_x = qn.coord_x;
    coord_y = qn.coord_y;
    lay_s   = qn.lay_s;
    lay_u   = qn.lay_u;
    lay_d   = qn.lay_d;
    lay_l   = qn.lay_l;
    lay_r   = qn.lay_r;
    qn.reset();
}

QuadNode::~QuadNode() noexcept {
    reset();
}

// bool QuadNode::is_root()    const { return parent == self; }
bool QuadNode::has_self()   const { return self   >= 0; }
// bool QuadNode::has_parent() const { return parent >= 0 && !is_root(); }
bool QuadNode::has_up()     const { return up     >= 0; }
bool QuadNode::has_down()   const { return down   >= 0; }
bool QuadNode::has_left()   const { return left   >= 0; }
bool QuadNode::has_right()  const { return right  >= 0; }

// bool QuadNode::really_has_up()    const { return has_up()    && up    != parent; }
// bool QuadNode::really_has_down()  const { return has_down()  && down  != parent; }
// bool QuadNode::really_has_left()  const { return has_left()  && left  != parent; }
// bool QuadNode::really_has_right() const { return has_right() && right != parent;}

int QuadNode::get_self()   const { return self;   }
// int QuadNode::get_parent() const { return parent; }
int QuadNode::get_up()     const { return up;     }
int QuadNode::get_down()   const { return down;   }
int QuadNode::get_left()   const { return left;   }
int QuadNode::get_right()  const { return right;  }
CoordPair QuadNode::get_coord() const { return CoordPair(coord_x, coord_y); }
int QuadNode::get_coord_x() const { return coord_x; }
int QuadNode::get_coord_y() const { return coord_y; }
int QuadNode::get_layer_self() const { return lay_s; }
int QuadNode::get_layer_up() const { return lay_u; }
int QuadNode::get_layer_down() const { return lay_d; }
int QuadNode::get_layer_left() const { return lay_l; }
int QuadNode::get_layer_right() const { return lay_r; }
unsigned QuadNode::get_flag() const { return flag; }

void QuadNode::set_self   (int s) { self   = s; }
// void QuadNode::set_parent (int p) { parent = p; }
void QuadNode::set_up     (int u) { up     = u; }
void QuadNode::set_down   (int d) { down   = d; }
void QuadNode::set_left   (int l) { left   = l; }
void QuadNode::set_right  (int r) { right  = r; }
void QuadNode::set_layer_self(int ls) { lay_s = ls; }
void QuadNode::set_layer_up(int lu) { lay_u = lu; }
void QuadNode::set_layer_down(int ld) { lay_d = ld; }
void QuadNode::set_layer_left(int ll) { lay_l = ll; }
void QuadNode::set_layer_right(int lr) { lay_r = lr; }

// void QuadNode::reset_node(int s, int p, int u, int d, int l, int r){
//     self = s; parent = p; up = u; down = d; left = l; right = r;
// }

void QuadNode::set_x      (int c_x) { coord_x = c_x; }
void QuadNode::set_y      (int c_y) { coord_y = c_y; }
void QuadNode::reset_coord(const CoordPair& c) { coord_x = c.first; coord_y = c.second; }
void QuadNode::reset_coord(int c_x, int c_y) { coord_x = c_x; coord_y = c_y; }

void QuadNode::reset(){
    self = up = down = left = right = coord_x = coord_y = lay_s = lay_u = lay_d = lay_l, lay_r = -1;
    flag = 0;
}
void QuadNode::reset(const QuadNode& qn){
    self    = qn.self;
    up      = qn.up;
    down    = qn.down;
    left    = qn.left;
    right   = qn.right;
    coord_x = qn.coord_x;
    coord_y = qn.coord_y;
    lay_s   = qn.lay_s;
    lay_u   = qn.lay_u;
    lay_d   = qn.lay_d;
    lay_l   = qn.lay_l;
    lay_r   = qn.lay_r;
}

void QuadNode::update_flag(unsigned _flag){ flag = _flag; }

void QuadNode::move_vertical(int _x)  { coord_x += _x; }
void QuadNode::move_horizontal(int _y){ coord_y += _y; }

unsigned QuadNode::dist(const CoordPair& c) const { // Manhattan distance
    return (unsigned)(ABS(coord_x - c.first) + ABS(coord_y - c.second));
}
unsigned QuadNode::dist(const QuadNode& qn) const { // Manhattan distance
    return (unsigned)(ABS(coord_x - qn.get_coord_x()) + ABS(coord_y - qn.get_coord_y()));
}

std::ostream& operator<<(std::ostream& out, const QuadNode& qn) {
    out << "QuadNode{s:" << qn.self
        <<        ", u:" << qn.up   << ", d:" << qn.down
        <<        ", l:" << qn.left << ", r:" << qn.right
        <<        ", lay_s: " << qn.lay_s
        << "}";
    return out;
}

SimpleEdge::SimpleEdge() noexcept 
    : v1(0), v2(0), weight(0) {}
SimpleEdge::SimpleEdge(unsigned _v1, unsigned _v2, unsigned _w) noexcept 
    : v1(_v1), v2(_v2), weight(_w) {
    if(v1 > v2) {
        std::swap(v1, v2);
    }
}

bool SimpleEdge::operator<(const SimpleEdge& se) const {
    if(weight != se.weight) return weight < se.weight;
    else if(v1 != se.v1)    return v1 < se.v1;
    else                    return v2 < se.v2;
}

unsigned SimpleEdge::get_v1()     const { return v1; }
unsigned SimpleEdge::get_v2()     const { return v2; }
unsigned SimpleEdge::get_weight() const { return weight; }
