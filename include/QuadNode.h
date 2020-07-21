// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>

#include "QuadUtil.h"

class QuadNode {
   public:
    // constructor
    QuadNode() noexcept;
    QuadNode(int idx) noexcept;
    QuadNode(int _s,
             int _u, int _d,
             int _l, int _r,
             int _x ,int _y,
             int _ls,
             int _lu, int _ld,
             int _ll, int _lr) noexcept;
    QuadNode(const QuadNode& qn) noexcept;
    QuadNode(QuadNode&& qn) noexcept;
    ~QuadNode() noexcept;

    QuadNode& operator=(const QuadNode& tn) noexcept;
    QuadNode& operator=(QuadNode&& tn) noexcept;

    bool is_pin()     const;
    // bool is_root()    const;
    bool has_self()   const;
    // bool has_parent() const;
    bool has_up()     const;
    bool has_down()   const;
    bool has_left()   const;
    bool has_right()  const;

    // bool really_has_up()    const;
    // bool really_has_down()  const;
    // bool really_has_left()  const;
    // bool really_has_right() const;

    int get_self()    const;
    int get_parent()  const;
    int get_up()      const;
    int get_down()    const;
    int get_left()    const;
    int get_right()   const;
    CoordPair get_coord() const;
    int get_coord_x() const;
    int get_coord_y() const;
    int get_layer_self()  const;
    int get_layer_up()    const;
    int get_layer_down()  const;
    int get_layer_left()  const;
    int get_layer_right() const;
    unsigned get_flag() const;

    void set_self  (int s);
    // void set_parent(int p);
    void set_up    (int u);
    void set_down  (int d);
    void set_left  (int l);
    void set_right (int r);
    void set_layer_self(int ls);
    void set_layer_up(int lu);
    void set_layer_down(int ld);
    void set_layer_left(int ll);
    void set_layer_right(int lr);
    // void reset_node(int s, int p, int u, int d, int l, int r);

    void set_x      (int c_x);
    void set_y      (int c_y);
    void reset_coord(const CoordPair& c);
    void reset_coord(int c_x, int c_y);
    // void set_layer (int l);

    void reset();
    void reset(const QuadNode& qn);

    void set_pin(int idx);
    void update_flag(unsigned _flag);

    void move_vertical(int _x);
    void move_horizontal(int _y);

    unsigned dist(const CoordPair& c) const;
    unsigned dist(const QuadNode& qn) const;

   private:
    int self, up, down, left, right;
    int coord_x, coord_y;
    int lay_s, lay_u, lay_d, lay_l, lay_r;
    int pin_idx;
    unsigned flag;

    // friends
    friend std::ostream& operator<<(std::ostream& out, const QuadNode& qn);
};

class SimpleEdge {
   public:
    SimpleEdge() noexcept;
    SimpleEdge(unsigned _v1, unsigned _v2, unsigned _w, unsigned _l) noexcept;

    bool operator<(const SimpleEdge& se) const;

    unsigned get_v1() const;
    unsigned get_v2() const;
    unsigned get_weight() const;
    unsigned get_layer() const;

   private:
    unsigned v1, v2, weight, layer;
};
