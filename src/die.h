/*
This file is to define a die for routing
 */

#include <queue>
#include <utility>
#include <vector>
#include "grid.h"

enum cost_type {
    Manhattan,

};

class MyComp {
   public:
    MyComp() {}
    bool operator()(const Grid* lhs, const Grid* rhs) const {
        return ((lhs->get_estimated()) > (rhs->get_estimated()));
    }
};

// typedef
typedef std::vector<Grid*> GridList;
typedef std::vector<unsigned> IdxList;
typedef std::priority_queue<Grid*, std::vector<Grid*>, MyComp> priority_grid;

// initialization

#ifndef DIE
#define DIE

class Die {
   public:
    // friend class of cost functions
    friend class CostGen;
    friend class ManDist;
    friend priority_grid;

    // public functions
    Die(unsigned x, unsigned y, unsigned z, cost_type t);
    ~Die();
    int A_star(const unsigned, IdxList&);
    bool add_net(const unsigned, const unsigned);
    void add_normal();

    // index functions
    unsigned get_x(unsigned idx) const { return (idx % _area) % _x; }
    unsigned get_y(unsigned idx) const { return (idx % _area) / _x; }
    unsigned get_z(unsigned idx) const { return idx / _area; }
    unsigned get_idx(unsigned x, unsigned y, unsigned z) const {
        return z * _area + y * _x + x;
    }

    // cost functions
    int get_cost(unsigned a, unsigned b);
    int ManDist(unsigned a, unsigned b) const;

   private:
    GridList _GridList;
    priority_grid* _PriorityGrid;
    cost_type _CostType;
    // constant
    const unsigned _x;
    const unsigned _y;
    const unsigned _area;
    const unsigned _z;
    const unsigned _vol;
    //

    // private functions
    bool propagate(unsigned, unsigned);
    bool sub_propagate(int, unsigned, unsigned, unsigned);
    void init_cost(cost_type);
    void backtrace(unsigned, IdxList&);
    void reset();
    void use_grid(unsigned);
    void clear_space(const unsigned);
    void revive(const unsigned);
    void revive_grid(const unsigned);
    void init_clear(const unsigned, const unsigned);
    void init_grid(const unsigned, const unsigned);
    // operator overloading
};

#endif