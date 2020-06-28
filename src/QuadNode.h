// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>

typedef std::pair<int, int> CoordPair; // Coordinate of points

class QuadNode {
   public:
    // constructor
    QuadNode() noexcept;
    QuadNode(int idx) noexcept;
    QuadNode(int _s, int _p,
             int _u, int _d,
             int _l, int _r,
             int _x ,int _y) noexcept;
    QuadNode(const QuadNode& qn) noexcept;
    QuadNode(QuadNode&& qn) noexcept;

    QuadNode& operator=(const QuadNode& tn) noexcept;
    QuadNode& operator=(QuadNode&& tn) noexcept;

    bool is_root()    const;
    bool has_self()   const;
    bool has_parent() const;
    bool has_up()     const;
    bool has_down()   const;
    bool has_left()   const;
    bool has_right()  const;

    int get_self()    const;
    int get_parent()  const;
    int get_up()      const;
    int get_down()    const;
    int get_left()    const;
    int get_right()   const;
    CoordPair get_coord() const;

    void set_self  (int s);
    void set_parent(int p);
    void set_up    (int u);
    void set_down  (int d);
    void set_left  (int l);
    void set_right (int r);
    void reset_node(int s, int p, int u, int d, int l, int r);

    void set_x      (int c_x);
    void set_y      (int c_y);
    void reset_coord(CoordPair c);
    void reset_coord(int c_x, int c_y);

   private:
    int self, parent, up, down, left, right;
    int coord_x, coord_y;

    // friends
    friend std::ostream& operator<<(std::ostream& out, const QuadNode& qn);
};

class SimpleEdge {
   public:
    SimpleEdge() noexcept;
    SimpleEdge(unsigned _v1, unsigned _v2, unsigned _w) noexcept;

    bool operator<(const SimpleEdge& se) const;

    unsigned get_v1() const;
    unsigned get_v2() const;
    unsigned get_weight() const;

   private:
    unsigned v1, v2, weight;
};