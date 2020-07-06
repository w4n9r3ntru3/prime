#include "router3d.h"

#include <iostream>


int grid::_global_search = 0;

Router3D::Router3D(Chip& pm) : _pm(pm), _PriorityGrid(nullptr) {
    size_t n = pm.getVolume();
    _GridList.reserve(n);
    for (size_t i = 0; i < n; i++) {
        std::shared_ptr<grid> g(new grid(i));
        _GridList.push_back(g);
    }
}

bool Router3D::A_star(const unsigned srow,
                      const unsigned scol,
                      const unsigned slay,
                      const unsigned erow,
                      const unsigned ecol,
                      const unsigned elay,
                      const bool allow_middle_point,
                      const GridNet& net,
                      safe::vector<unsigned>& ans,
                      cost_type t)  // return the cost of the rout
{
    _CostType = t;
    grid::_global_search++;
    if (_PriorityGrid) {
        delete _PriorityGrid;
    }
    _PriorityGrid = new priority_grid;
    const unsigned origin = get_idx(srow, scol, slay);
    unsigned x = origin;
    const unsigned target = get_idx(erow, ecol, elay);
    while (!propagate(x, target, allow_middle_point, net)) {
        if (_PriorityGrid->empty()) {
            return 0;
        }
        x = (_PriorityGrid->top())->get_idx();
        int pi = _GridList[x]->get_pi();
        int cost = _GridList[pi]->get_cost();
        // std::cout << x << " " << cost << std::endl;
        _PriorityGrid->pop();
        _GridList[x]->assign_cost(cost + 1);
    }
    backtrace(target, origin, ans);
    return true;
}

bool Router3D::propagate(const unsigned pi,
                         const unsigned target,
                         const bool allow_middle_point,
                         const GridNet& net)  // true means find target
{
    unsigned x = get_row(pi);
    unsigned y = get_column(pi);
    unsigned z = get_layer(pi);
    int cost = _GridList[pi]->get_cost();
    if (z >= net.getMinlayer()) {
        if (_pm.getLayer(z).getDirection()) {
            if (x > 0)  // left
            {
                if (sub_propagate(cost, pi, pi - 1, target, allow_middle_point,
                                  net))
                    return true;
            }
            if (x < (_pm.getNumRows() - 1))  // right
            {
                if (sub_propagate(cost, pi, pi + 1, target, allow_middle_point,
                                  net))
                    return true;
            }
        } else {
            if (y > 0)  // back
            {
                if (sub_propagate(cost, pi, pi - _pm.getNumRows(), target,
                                  allow_middle_point, net))
                    return true;
            }
            if (y < (_pm.getNumColumns() - 1))  // front
            {
                if (sub_propagate(cost, pi, pi + _pm.getNumRows(), target,
                                  allow_middle_point, net))
                    return true;
            }
        }
    }
    if (z > 0)  // down
    {
        if (sub_propagate(cost, pi, pi - _pm.getArea(), target,
                          allow_middle_point, net))
            return true;
    }
    if (z < (_pm.getNumLayers() - 1))  // up
    {
        if (sub_propagate(cost, pi, pi + _pm.getArea(), target,
                          allow_middle_point, net))
            return true;
    }
    return false;
}

inline bool Router3D::sub_propagate(
    const int x,
    const unsigned pi,
    const unsigned front,
    const unsigned target,
    const bool allow_middle_point,
    const GridNet& net)  // true means find target and x is pi's cost
{
    Grid& g = _pm.getGrid(get_idx_area(front), get_layer(front));
    if ((front == target) || (g.canGetNet(net) && allow_middle_point)) {
        _GridList[front]->assign_estimated(
            pi, x + 1);  // actually it should be assign_cost but for the
                         // convenience of virtual function
        return true;
    }
    if (_GridList[front]->routable() && (g.getSupply() > 0)) {
        _GridList[front]->assign_estimated(pi, x + 1 + get_cost(front, target));
        _PriorityGrid->push(_GridList[front]);
        return false;
    }
    return false;
}

void Router3D::backtrace(const unsigned target,
                         const unsigned origin,
                         IdxList& ans) {
    unsigned x = target;
    ans.push_back(get_row(x));
    ans.push_back(get_column(x));
    ans.push_back(get_layer(x));
    while (x != origin) {
        x = _GridList[x]->get_pi();
        ans.push_back(get_row(x));
        ans.push_back(get_column(x));
        ans.push_back(get_layer(x));
    }
}


bool L_shape(
        const unsigned srow,
        const unsigned scol,
        const unsigned slay,
        const unsigned erow,
        const unsigned ecol,
        const unsigned elay,
        const bool allow_middle_point,  // true if you want to return when find
                                        // a middle point of the same net
        const GridNet& net,
        IdxList& ans);