#include "router3d.h"
#include <cmath>

bool Router3D::L_shape(
    const unsigned srow,
    const unsigned scol,
    const unsigned slay,
    const unsigned erow,
    const unsigned ecol,
    const unsigned elay,
    const GridNet& net,
    IdxList& ans) {
    std::vector<int> cost; // -1 means unavailable
    cost.reserve(abs(srow-erow)+abs(scol-ecol));
    std::vector<unsigned> pi;
    pi.reserve(abs(srow-erow)+abs(scol-ecol));

    unsigned iter = 0;
    for (unsigned i = 0; i < _pm.getNumLayers(); ++i) {
        if (_pm.getGrid(i,_pm.getIdx(srow+1,scol)))
    }
    if(srow < erow) {
        for (unsigned i = srow + 2; i < erow; i) {
            for ()
        }
    }
}