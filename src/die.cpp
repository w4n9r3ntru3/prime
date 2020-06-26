#include "die.h"
#include "iostream"

Die::Die(unsigned x, unsigned y, unsigned z, cost_type t)
    : _x(x), _y(y), _z(z), _area(_x * _y), _vol(_x * _y * _z), _CostType(t) {
    _GridList.reserve(_vol);
    for (int i = 0; i < _vol; i++) {
        Grid* grid = 0;
        _GridList.push_back(grid);
    }
}

Die::~Die() {
    for (int i = 0; i < _vol; i++)
        delete _GridList[i];
}

bool Die::add_net(unsigned idx1, unsigned idx2) {
    if (_GridList[idx1] != 0 || _GridList[idx2] != 0)
        return false;  // idx initialized
    _GridList[idx1] = new StartGrid(idx1, idx2);
    _GridList[idx2] = new EndGrid(idx2);
    init_clear(idx1, idx2);
    init_clear(idx2, idx2);
    return true;
    //_IdxList.push_back(net[0]);
    // for(int i = 1;i < net.size();i++)
    //{
    //    _GridList->add_end(net[i]);
    //    Grid* end = _GridList[net[i]];
    //    if(end !=0) return false;//idx initialized
    //    end = new EndGrid();
    //}
}

void Die::add_normal() {
    for (int i = 0; i < _vol; i++) {
        if (_GridList[i] == 0)
            _GridList[i] = new NormalGrid(i);
    }
}

int Die::A_star(const unsigned origin,
                IdxList& ans)  // return the cost of the rout
{
    _PriorityGrid = new priority_grid;
    unsigned x = origin;
    const unsigned target = _GridList[x]->get_target();
    while (!propagate(x, target)) {
        if (_PriorityGrid->empty()) {
            std::cout << "Network " << get_x(origin) << " " << get_y(origin)
                      << " " << get_z(origin) << " " << get_x(target) << " "
                      << get_y(target) << " " << get_z(target)
                      << " has no solution." << std::endl;
            clear_space(origin);
            clear_space(target);
            reset();
            return 0;
        }
        x = (_PriorityGrid->top())->get_idx();
        int pi = _GridList[x]->get_target();
        int cost = _GridList[pi]->get_cost();
        // std::cout << x << " " << cost << std::endl;
        _PriorityGrid->pop();
        _GridList[x]->assign_cost(cost + 1);
    }
    backtrace(target, ans);
    int ret = ans.size() - 2;  // ans.size()-2;
    for (int i = 0; i < ans.size(); i++)
        clear_space(ans[i]);
    reset();
    return ret;
}

void Die::init_clear(const unsigned pi, const unsigned target) {
    unsigned x = get_x(pi);
    unsigned y = get_y(pi);
    unsigned z = get_z(pi);
    if (x > 0)  // left
        init_grid(pi - 1, target);
    if (x < (_x - 1))  // right
        init_grid(pi + 1, target);
    if (y > 0)  // back
    {
        init_grid(pi - _x, target);
        if (x > 0)  // leftback
            init_grid(pi - _x - 1, target);
        if (x < (_x - 1))  // rightback
            init_grid(pi - _x + 1, target);
    }
    if (y < _y - 1)  // front
    {
        init_grid(pi + _x, target);
        if (x > 0)  // leftfront
            init_grid(pi + _x - 1, target);
        if (x < (_x - 1))  // rightfront
            init_grid(pi + _x + 1, target);
    }
    if (z > 0)  // down
        init_grid(pi - _area, target);
    if (z < _z - 1)  // up
        init_grid(pi + _area, target);
}

void Die::init_grid(const unsigned x, const unsigned target) {
    if (_GridList[x] == 0)
        _GridList[x] = new NormalGrid(x, target);
    else {
        delete _GridList[x];
        _GridList[x] = new BlockGrid(x);
    }
}

bool Die::propagate(const unsigned pi,
                    const unsigned target)  // true means find target
{
    unsigned x = get_x(pi);
    unsigned y = get_y(pi);
    unsigned z = get_z(pi);
    int cost = _GridList[pi]->get_cost();
    if (x > 0)  // left
    {
        if (sub_propagate(cost, pi, pi - 1, target))
            return true;
    }
    if (x < (_x - 1))  // right
    {
        if (sub_propagate(cost, pi, pi + 1, target))
            return true;
    }
    if (y > 0)  // back
    {
        if (sub_propagate(cost, pi, pi - _x, target))
            return true;
    }
    if (y < _y - 1)  // front
    {
        if (sub_propagate(cost, pi, pi + _x, target))
            return true;
    }
    if (z > 0)  // down
    {
        if (sub_propagate(cost, pi, pi - _area, target))
            return true;
    }
    if (z < _z - 1)  // up
    {
        if (sub_propagate(cost, pi, pi + _area, target))
            return true;
    }
    return false;
}

inline bool Die::sub_propagate(
    const int x,
    const unsigned pi,
    const unsigned front,
    const unsigned target)  // true means find target and x is pi's cost
{
    if (front == target) {
        _GridList[front]->assign_estimated(
            pi, x + 1);  // actually it should be assign_cost but for the
                         // convenience of virtual function
        return true;
    }
    if (_GridList[front]->routable(target)) {
        _GridList[front]->assign_estimated(pi, x + 1 + get_cost(front, target));
        _PriorityGrid->push(_GridList[front]);
        return false;
    }
    return false;
}

void Die::reset() {
    delete _PriorityGrid;
    for (int i = 0; i < _vol; i++)
        _GridList[i]->reset();
}

void Die::backtrace(const unsigned target, IdxList& ans) {
    unsigned x = target;
    unsigned y;
    ans.push_back(x);
    while (_GridList[x]->type() != Start_Grid) {
        y = x;
        x = _GridList[x]->get_target();
        ans.push_back(x);
        use_grid(y);
    }
}

inline void Die::use_grid(unsigned x) {
    // if(_GridList[x] == 0) _GridList[x] = new BlockGrid(x);
    if (_GridList[x]->type() == Normal_Grid) {
        delete _GridList[x];
        _GridList[x] = new BlockGrid(x);
    }
    /*if(_GridList[x]->type() == Block_Grid)
    {
        delete _GridList[x];
        _GridList[x] = new ReserveGrid(x);
    }*/
}

void Die::clear_space(const unsigned pi) {
    unsigned x = get_x(pi);
    unsigned y = get_y(pi);
    unsigned z = get_z(pi);
    if (x > 0)  // left
        use_grid(pi - 1);
    if (x < (_x - 1))  // right
        use_grid(pi + 1);
    if (y > 0)  // back
    {
        use_grid(pi - _x);
        if (x > 0)  // leftback
            use_grid(pi - _x - 1);
        if (x < (_x - 1))  // rightback
            use_grid(pi - _x + 1);
    }
    if (y < _y - 1)  // front
    {
        use_grid(pi + _x);
        if (x > 0)  // leftfront
            use_grid(pi + _x - 1);
        if (x < (_x - 1))  // rightfront
            use_grid(pi + _x + 1);
    }
    if (z > 0)  // down
        use_grid(pi - _area);
    if (z < _z - 1)  // up
        use_grid(pi + _area);
}

void Die::revive(const unsigned pi) {
    unsigned x = get_x(pi);
    unsigned y = get_y(pi);
    unsigned z = get_z(pi);
    if (x > 0)  // left
        revive_grid(pi - 1);
    if (x < (_x - 1))  // right
        revive_grid(pi + 1);
    if (y > 0)  // back
    {
        revive_grid(pi - _x);
        if (x > 0)  // leftback
            revive_grid(pi - _x - 1);
        if (x < (_x - 1))  // rightback
            revive_grid(pi - _x + 1);
    }
    if (y < _y - 1)  // front
    {
        revive_grid(pi + _x);
        if (x > 0)  // leftfront
            revive_grid(pi + _x - 1);
        if (x < (_x - 1))  // rightfront
            revive_grid(pi + _x + 1);
    }
    if (z > 0)  // down
        revive_grid(pi - _area);
    if (z < _z - 1)  // up
        revive_grid(pi + _area);
}

void Die::revive_grid(const unsigned x) {
    if (_GridList[x]->type() == Block_Grid) {
        delete _GridList[x];
        _GridList[x] = new NormalGrid(x);
    }
}