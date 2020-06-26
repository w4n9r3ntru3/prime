// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>

class QuadNode {
   public:
    // constructor
    QuadNode() noexcept;
    QuadNode(unsigned idx) noexcept;
    QuadNode(unsigned self, unsigned parent,
             unsigned left, unsigned right,
             unsigned up  , unsigned down) noexcept;
    QuadNode(const QuadNode& qn) noexcept;
    QuadNode(QuadNode&& qn) noexcept;

    QuadNode& operator=(const QuadNode& tn) noexcept;
    QuadNode& operator=(QuadNode&& tn) noexcept;

    bool has_self()   const;
    bool has_parent() const;
    bool has_left()   const;
    bool has_right()  const;
    bool has_up()     const;
    bool has_down()   const;

    unsigned get_self()   const;
    unsigned get_parent() const;
    unsigned get_left()   const;
    unsigned get_right()  const;
    unsigned get_up()     const;
    unsigned get_down()   const;

    void set_self  (unsigned s);
    void set_parent(unsigned p);
    void set_left  (unsigned l);
    void set_right (unsigned r);
    void set_up    (unsigned u);
    void set_down  (unsigned d);

   private:
    int self, parent, left, right, up, down;

    // friends
    friend std::ostream& operator<<(std::ostream& out, const QuadNode& qn);
};

class SimpleEdge {
   public:
    SimpleEdge() noexcept;
    SimpleEdge(unsigned _v1, unsigned _v2, unsigned _w) noexcept;

    const bool operator<(const SimpleEdge& se) const;

    const unsigned get_v1() const;
    const unsigned get_v2() const;
    const unsigned get_weight() const;

   private:
    unsigned v1, v2, weight;
};