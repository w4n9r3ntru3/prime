/***********************************************************************

  FileName    [L_shape.cpp]

  Author      [Yang Chien Yi]

  This file implements the L_shape router.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <climits>
#include <cmath>

#include "router3d.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

bool isOdd(int a) {
    return a & 01;
}

bool isEven(int a) {
    return !isOdd(a);
}

bool Router3D::L_shape(const unsigned srow,
                       const unsigned scol,
                       const unsigned slay,
                       const unsigned erow,
                       const unsigned ecol,
                       const unsigned elay,
                       const GridNet& net,
                       IdxList& ans) {
    unsigned layH1, layV1, layH2, layV2;
    int minSupply1, minSupply2, HV, VH;
    bool HVavailable, VHavailable;
    // H-V L shape
    safe::vector<int> minSupplyTable1(_pm.getNumLayers(),
                                      -1);  // -1 means not available
    safe::vector<unsigned> cost1(_pm.getNumLayers(),
                                 UINT_MAX);  // MAX means not available
    HVavailable = Rout_H(slay, srow, scol, ecol, net, minSupplyTable1, cost1) &&
                  Rout_V(elay, ecol, erow, srow, net, minSupplyTable1, cost1);
    if (HVavailable) {
        HV = Select(minSupplyTable1, cost1, layH1, layV1, minSupply1);
    }

    // V-H L shape
    safe::vector<int> minSupplyTable2(_pm.getNumLayers(),
                                      -1);  // -1 means not available
    safe::vector<unsigned> cost2(_pm.getNumLayers(),
                                 UINT_MAX);  // MAX means not available
    VHavailable = Rout_V(slay, scol, srow, erow, net, minSupplyTable2, cost2) &&
                  Rout_H(elay, erow, ecol, scol, net, minSupplyTable2, cost2);
    if (VHavailable) {
        VH = Select(minSupplyTable1, cost1, layH2, layV2, minSupply2);
    }

    // return ans
    if (HVavailable || VHavailable) {
        ans.reserve(30);
        if (HV < VH || (HV == VH && (minSupply2 <= minSupply1))) {
            if (slay != layH1) {  // first via
                assignWire(srow, scol, slay, srow, scol, layH1, ans);
            }
            if (scol != ecol) {  // H
                assignWire(srow, scol, layH1, srow, ecol, layH1, ans);
            }
            if (layH1 != layV1) {  // second via
                assignWire(srow, ecol, layH1, srow, ecol, layV1, ans);
            }
            if (srow != erow) {  // V
                assignWire(srow, ecol, layV1, erow, ecol, layV1, ans);
            }
            if (layV1 != elay) {  // last via
                assignWire(erow, ecol, layV1, erow, ecol, elay, ans);
            }
        } else {
            if (slay != layV2) {  // first via
                assignWire(srow, scol, slay, srow, scol, layV2, ans);
            }
            if (srow != erow) {  // H
                assignWire(srow, scol, layV2, erow, scol, layV2, ans);
            }
            if (layV2 != layH2) {  // second via
                assignWire(erow, scol, layV2, erow, scol, layH2, ans);
            }
            if (scol != ecol) {  // V
                assignWire(erow, scol, layH2, erow, ecol, layH2, ans);
            }
            if (layV1 != elay) {  // last via
                assignWire(erow, ecol, layH2, erow, ecol, elay, ans);
            }
        }
        return true;
    } else {
        return false;
    }
}

inline bool Router3D::Layer_Assignment_H(const unsigned scol,
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

inline bool Router3D::Layer_Assignment_V(const unsigned srow,
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

inline bool Router3D::Via_Assignment(const unsigned slay,
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

inline bool Router3D::Rout_H(const unsigned lay,
                             const unsigned row,
                             const unsigned scol,
                             const unsigned ecol,
                             const GridNet& net,
                             safe::vector<int>& minSupplyTable,
                             safe::vector<unsigned>& cost) {
    // H down
    bool ret = false;
    for (int i = lay, j = lay; i >= net.getMinlayer();
         (isEven(i)) ? i -= 1 : i -= 2) {
        if (isEven(i)) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, row, scol, net, minSupply) &&
            Layer_Assignment_H(scol, ecol, row, i, net, minSupply)) {
            ret = true;
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
    // H up
    for (int i = lay, j = lay; i < _pm.getNumLayers() && i >= net.getMinlayer();
         (isEven(i)) ? i += 1 : i += 2) {
        if (isEven(i)) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, row, scol, net, minSupply) &&
            Layer_Assignment_H(scol, ecol, row, i, net, minSupply)) {
            ret = true;
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
}

inline bool Router3D::Rout_V(const unsigned lay,
                             const unsigned col,
                             const unsigned srow,
                             const unsigned erow,
                             const GridNet& net,
                             safe::vector<int>& minSupplyTable,
                             safe::vector<unsigned>& cost) {
    bool ret = false;
    // V down
    for (int i = lay, j = lay; i >= net.getMinlayer();
         (isOdd(i)) ? i -= 2 : i -= 1) {
        if (isOdd(i)) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, erow, col, net, minSupply) &&
            Layer_Assignment_V(srow, erow, col, i, net, minSupply)) {
            ret = true;
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
    // V up
    for (int i = lay, j = lay; i < _pm.getNumLayers() && i >= net.getMinlayer();
         (isOdd(i)) ? i += 2 : i += 1) {
        if (!(isOdd(i))) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, erow, col, net, minSupply) &&
            Layer_Assignment_V(srow, erow, col, i, net, minSupply)) {
            ret = true;
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
}

inline unsigned Router3D::Select(const safe::vector<int>& minSupplyTable,
                                 const safe::vector<unsigned>& cost,
                                 unsigned& layH,
                                 unsigned& layV,
                                 int& minSupply) {
    assert(minSupplyTable.size() == cost.size());
    unsigned ret = UINT_MAX;
    minSupply = INT_MAX;
    for (unsigned i = 0; i < _pm.getNumLayers(); i += 2) {
        for (unsigned j = 1; j < _pm.getNumLayers(); j += 2) {
            int c = cost[i] + cost[j];
            if (c < ret) {
                layH = i;
                layV = j;
                ret = c;
                minSupply = minSupplyTable[i] < minSupplyTable[j]
                                ? minSupplyTable[i]
                                : minSupplyTable[j];
            } else if (c == ret) {
                int min = minSupplyTable[i] < minSupplyTable[j]
                              ? minSupplyTable[i]
                              : minSupplyTable[j];
                if (min > minSupply) {
                    minSupply = min;
                    layH = i;
                    layV = j;
                }
            }
        }
    }
    return ret;
}

void Router3D::assignWire(const unsigned srow,
                          const unsigned scol,
                          const unsigned slay,
                          const unsigned erow,
                          const unsigned ecol,
                          const unsigned elay,
                          IdxList& ans) {
    ans.push_back(srow);
    ans.push_back(scol);
    ans.push_back(slay);
    ans.push_back(erow);
    ans.push_back(ecol);
    ans.push_back(elay);
}