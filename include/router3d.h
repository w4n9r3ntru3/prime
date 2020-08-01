/***********************************************************************

  FileName    [router.h]

  Author      [Yang Chien Yi]

  This file difines the router set. A_star.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <utility>

#include "Chip.h"
#include "safe.h"

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
    bool operator()(const std::shared_ptr<grid> lhs,
                    const std::shared_ptr<grid> rhs) const {
        return ((lhs->get_estimated()) > (rhs->get_estimated()));
    }
};

// typedef
typedef safe::vector<std::shared_ptr<grid>> GridList;
typedef safe::vector<unsigned> IdxList;
typedef std::priority_queue<std::shared_ptr<grid>, GridList, MyComp>
    priority_grid;

class Router3D {
   public:
    // interface functions
    Router3D(Chip& pm);
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
        cost_type t);  // start, end, net, return route, cost type; true if
                       // there is a route, false if no

    bool L_shape(const unsigned srow,
                 const unsigned scol,
                 const unsigned slay,
                 const unsigned erow,
                 const unsigned ecol,
                 const unsigned elay,
                 const GridNet& net,
                 IdxList& ans);

    bool Layer_Assignment_H(const unsigned scol,
                            const unsigned ecol,
                            const unsigned row,
                            const unsigned lay,
                            const GridNet& net,
                            int& minSupply);

    bool Layer_Assignment_V(const unsigned srow,
                            const unsigned erow,
                            const unsigned col,
                            const unsigned lay,
                            const GridNet& net,
                            int& minSupply);

    bool Via_Assignment(const unsigned slay,
                        const unsigned elay,
                        const unsigned row,
                        const unsigned col,
                        const GridNet& net,
                        int& minSupply);

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

    // A_star functions
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
