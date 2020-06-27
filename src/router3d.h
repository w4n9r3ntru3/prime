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

#include "Chip.h"

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
    grid(unsigned i) : _idx(i), _search(0) {}
    ~grid() {}

    // access functions
    unsigned get_idx() const { return _idx; }
    bool routable() const { return _global_search != _search; }
    int get_cost() const { return _cost; }
    int get_estimated() const { return _estimated_cost; }
    unsigned get_pi() const { return _pi; }

    // modifier
    void assign_estimated(unsigned pi, int e) {
        _pi = pi;
        _estimated_cost = e;
        _search = _global_search;
    }
    void assign_cost(int cost) { _cost = cost; }

    static int _global_search;

   private:
    unsigned _idx;
    unsigned _pi;
    int _cost;
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
    // interface functions
    Router3D(Chip& pm);
    ~Router3D();
    bool A_star(
        const unsigned srow,
        const unsigned scol,
        const unsigned slay,
        const unsigned erow,
        const unsigned ecol,
        const unsigned elay,
        const bool allow_middle_point,  // true if you want to return when find
                                        // a middle point of the same net
        const GridNet& net,
        IdxList& ans,  // row, column, layer
        cost_type t);  // start, end, net, return route, cost type; true if there is a route, false if no

    // friend class of cost functions
    friend class CostGen;
    friend class ManDist;
    friend priority_grid;

   private:
    Chip& _pm;
    GridList _GridList;
    priority_grid* _PriorityGrid;
    cost_type _CostType;

    // cost functions
    int get_cost(unsigned a, unsigned b);
    int ManDist(unsigned a, unsigned b) const;

    // idx functions
    unsigned get_row(unsigned idx) const {
        return (idx % _pm.getArea()) % _pm.getNumRows();
    }
    unsigned get_column(unsigned idx) const {
        return (idx % _pm.getArea()) / _pm.getNumRows();
    }
    unsigned get_layer(unsigned idx) const { return idx / _pm.getArea(); }
    unsigned get_idx(unsigned row, unsigned column, unsigned layer) const {
        return layer * _pm.getArea() + column * _pm.getNumRows() + row;
    }
    unsigned get_idx_area(unsigned idx) const { return idx % _pm.getArea(); }

    // private functions
    bool propagate(const unsigned, const unsigned, const bool, const GridNet&);
    bool sub_propagate(const int,
                       const unsigned,
                       const unsigned,
                       const unsigned,
                       const bool,
                       const GridNet&);
    void backtrace(const unsigned, const unsigned, IdxList&);
    // operator overloading
};