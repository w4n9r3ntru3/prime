#include <climits>
#include <cmath>

#include "router3d.h"

bool Router3D::L_shape(const unsigned srow,
                       const unsigned scol,
                       const unsigned slay,
                       const unsigned erow,
                       const unsigned ecol,
                       const unsigned elay,
                       const GridNet& net,
                       IdxList& ans) {
    // H-V L shape
    safe::vector<int> table(_pm.getNumLayers(), -1);
    safe::vector<unsigned> cost(_pm.getNumLayers(), UINT_MAX);
    // H
    for (int i = slay; i > 0; (i ^ 01) ? i -= 1 : i -= 2) {
        int minSupply = INT_MAX;
        if (Via_Assignment(slay, i, srow, scol, net, minSupply) &&
            Layer_Assignment_H(scol, ecol, srow, i, net, minSupply)) {
            table[i] = minSupply;
        }
    }
    for (int i = slay; i < _pm.getNumLayers(); (i ^ 01) ? i += 1 : i += 2) {
        int minSupply = INT_MAX;
        if (Via_Assignment(slay, i, srow, scol, net, minSupply) &&
            Layer_Assignment_H(scol, ecol, srow, i, net, minSupply)) {
            table[i] = minSupply;
        }
    }
    // V
    for (unsigned i = 1; i < _pm.getNumLayers(); i += 2) {
        int minSupply = INT_MAX;
        if (Via_Assignment(slay, i, srow, scol, net, minSupply) &&
            Layer_Assignment_H(scol, ecol, srow, i, net, minSupply)) {
            table[i] = minSupply;
        }
    }
}

bool Router3D::Layer_Assignment_H(const unsigned scol,
                                  const unsigned ecol,
                                  const unsigned row,
                                  const unsigned lay,
                                  const GridNet& net,
                                  int& minSupply) {
    for (unsigned i = scol; i <= ecol; (scol < ecol) ? ++i : --i) {
        Grid& grid = _pm.getGrid(lay, _pm.getIdx(row, i));
        if (!grid.routable(net)) {
            return false;
        }
        int supply = grid.getSupply();
        if (supply < minSupply) {
            minSupply = supply;
        }
    }
    return true;
}

bool Router3D::Layer_Assignment_V(const unsigned srow,
                                  const unsigned erow,
                                  const unsigned col,
                                  const unsigned lay,
                                  const GridNet& net,
                                  int& minSupply) {
    for (unsigned i = srow; i <= erow; (srow < erow) ? ++i : --i) {
        Grid& grid = _pm.getGrid(lay, _pm.getIdx(i, col));
        if (!grid.routable(net)) {
            return false;
        }
        int supply = grid.getSupply();
        if (supply < minSupply) {
            minSupply = supply;
        }
    }
    return true;
}

bool Router3D::Via_Assignment(const unsigned slay,
                              const unsigned elay,
                              const unsigned row,
                              const unsigned col,
                              const GridNet& net,
                              int& minSupply) {
    for (unsigned i = slay; i <= elay; (slay < elay) ? ++i : --i) {
        Grid& grid = _pm.getGrid(i, _pm.getIdx(row, col));
        if (!grid.routable(net)) {
            return false;
        }
        int supply = grid.getSupply();
        if (supply < minSupply) {
            minSupply = supply;
        }
    }
    return true;
}