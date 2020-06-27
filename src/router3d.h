/***********************************************************************

  FileName    [router.h]

  Author      [Yang Chien Yi]

  This file difines the router set. A_star.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <queue>
#include <utility>
#include <vector>

#include "PrimeMan.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

enum cost_type {
    Manhattan,

};

// initialization

class grid {
   public:
    grid() : _search(0) {}
    ~grid() {}

    bool routable(const unsigned) const { return _global_search != _search; }

    int get_cost() const { return _cost; }

    int get_estimated() const { return _estimated_cost; }

    void assign_estimated(unsigned pi, int e) {
        _pi = pi;
        _estimated_cost = e;
    }

    void assign_cost(int cost) { _cost = cost; }

    static int _global_search;

   private:
    int _cost;
    unsigned _pi;
    unsigned _pin;
    int _estimated_cost;
    int _search;
};

class MyComp {
   public:
    MyComp() {}
    bool operator()(const grid* lhs, const grid* rhs) const {
        return ((lhs->get_estimated()) > (rhs->get_estimated()));
    }
};

// typedef
typedef std::vector<grid*> GridList;
typedef std::vector<unsigned> IdxList;
typedef std::priority_queue<grid*, GridList, MyComp> priority_grid;

class Router3D {
   public:
    // friend class of cost functions
    friend class CostGen;
    friend class ManDist;
    friend priority_grid;

    // public functions
    Router3D(PrimeMan& pm);
    ~Router3D();
    int A_star(unsigned srow,
               unsigned scol,
               unsigned slay,
               unsigned erow,
               unsigned ecol,
               unsigned elay,
               GridNet& net);

    // cost functions
    int get_cost(unsigned a, unsigned b);
    int ManDist(unsigned a, unsigned b) const;

   private:
    PrimeMan& _pm;
    GridList _GridList;
    priority_grid* _PriorityGrid;
    cost_type _CostType;
    // constant
    const unsigned _area;
    //

    // private functions
    bool propagate(unsigned srow,
                   unsigned scol,
                   unsigned slay,
                   unsigned erow,
                   unsigned ecol,
                   unsigned elay,
                   GridNet& net);
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