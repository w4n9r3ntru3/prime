/***********************************************************************

  FileName    [Grid.cpp]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file describes the Layers, the Grids, and their Coordinates(to
  save cells).

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Grid.h"

#include <assert.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

// Layer
Layer::Layer(const std::string name,
             int i,
             bool d,
             int supply,
             int area) noexcept
    : _LayerName(name), _idx(i), _direction(d) {
    _grids.reserve(area);
    for (int i = 0; i < area; ++i) {
        Grid* g = new Grid(supply, *this);
        _grids.push_back(g);
    }
}

Layer::Layer(const Layer& l) noexcept
    : _LayerName(l._LayerName),
      _idx(l._idx),
      _direction(l._direction),
      _grids(l._grids) {}

Layer::~Layer() {
    for (int i = 0, n = _grids.size(); i < n; ++i) {
        delete _grids[i];
    }
}

const std::string& Layer::getLayerName() const {
    return _LayerName;
}

int Layer::getLayerIdx() const {
    return _idx;
}

Grid& Layer::getGrid(int i) {
    return *_grids[i];
}

// Coordinate
Coordinate::Coordinate(int x, int y, int layer) : _row(x), _column(y) {
    _grids.reserve(layer);
}

void Coordinate::addAdjH(Coordinate* c1, Coordinate* c2) {
    _c1 = c1;
    _c2 = c2;
}

void Coordinate::addGrid(Grid* g) {
    _grids.push_back(g);
}

bool Coordinate::CanAddCell(Cell& cell) const {
    // TODO verify fixed
    unsigned id = cell.getMasterCellId();
    unsigned n;
    if (_MCT2Num.contains(id)) {
        assert(_MCT2Num.at(id) >= 1);
        n = _MCT2Num.at(id) + 1;
    } else {
        n = 1;
    }
    for (int i = 0, m = _grids.size(); i < m; ++i) {
        int d = cell.getLayerDemand(i);
        safe::vector<unsigned>& sameGridMC = cell.getSameGridMC(i);
        safe::vector<int>& sameGridDemand = cell.getSameGridDemand(i);
        safe::vector<unsigned>& adjHGridMC = cell.getadjHGridMC(i);
        safe::vector<int>& adjHGridDemand = cell.getadjHGridDemand(i);
        assert(sameGridMC.size() == sameGridDemand.size());
        assert(adjHGridMC.size() == adjHGridDemand.size());
        for (int j = 0, o = sameGridMC.size(); j < o; ++j) {
            if (_MCT2Num.contains(sameGridMC[j])) {
                if (n <= _MCT2Num.find(sameGridMC[j])->second) {
                    d += sameGridDemand[j];
                }
            }
        }
        for (int j = 0, o = adjHGridMC.size(); j < o; ++j) {
            int k = adjHGridDemand[j];
            if (_c1) {
                if (_c1->_MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= _c1->_MCT2Num.find(adjHGridMC[j])->second) {
                        if (_c1->_grids[i]->getSupply() < k) {
                            return false;
                        }
                        d += k;
                    }
                }
            }
            if (_c2) {
                if (_c2->_MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= _c2->_MCT2Num.find(adjHGridMC[j])->second) {
                        if (_c2->_grids[i]->getSupply() < k) {
                            return false;
                        }
                        d += k;
                    }
                }
            }
        }
        if (_grids[i]->getSupply() < d) {
            return false;
        }
    }
    return true;
}

void Coordinate::addCell(Cell& cell) {
    unsigned id = cell.getMasterCellId();
    unsigned n;
    if (_MCT2Num.contains(id)) {
        assert(_MCT2Num[id] >= 1);
        n = ++_MCT2Num[id];
    } else {
        n = 1;
        _MCT2Num[id] = 1;
    }
    for (int i = 0, m = _grids.size(); i < m; ++i) {
        int d = cell.getLayerDemand(i);
        safe::vector<unsigned>& sameGridMC = cell.getSameGridMC(i);
        safe::vector<int>& sameGridDemand = cell.getSameGridDemand(i);
        safe::vector<unsigned>& adjHGridMC = cell.getadjHGridMC(i);
        safe::vector<int>& adjHGridDemand = cell.getadjHGridDemand(i);
        assert(sameGridMC.size() == sameGridDemand.size());
        assert(adjHGridMC.size() == adjHGridDemand.size());
        for (int j = 0, p = sameGridMC.size(); j < p; ++j) {
            if (_MCT2Num.contains(sameGridMC[j])) {
                if (n <= _MCT2Num[sameGridMC[j]]) {
                    d += sameGridDemand[j];
                }
            }
        }
        for (int j = 0, p = sameGridMC.size(); j < p; ++j) {
            int k = adjHGridDemand[j];
            if (_c1) {
                if (_c1->_MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= _c1->_MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        _c1->_grids[i]->decSupply(k);
                    }
                }
            }
            if (_c2) {
                if (_c2->_MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= _c2->_MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        _c2->_grids[i]->decSupply(k);
                    }
                }
            }
        }
        _grids[i]->decSupply(d);
    }
}

void Coordinate::moveCell(Cell& cell) {
    unsigned id = cell.getMasterCellId();
    assert(_MCT2Num.contains(id));
    unsigned n = --_MCT2Num[id];
    if (n == 0) {
        _MCT2Num.erase(id);
    }
    for (int i = 0, m = _grids.size(); i < m; ++i) {
        int d = cell.getLayerDemand(i);
        safe::vector<unsigned>& sameGridMC = cell.getSameGridMC(i);
        safe::vector<int>& sameGridDemand = cell.getSameGridDemand(i);
        safe::vector<unsigned>& adjHGridMC = cell.getadjHGridMC(i);
        safe::vector<int>& adjHGridDemand = cell.getadjHGridDemand(i);
        assert(sameGridMC.size() == sameGridDemand.size());
        assert(adjHGridMC.size() == adjHGridDemand.size());
        for (int j = 0, p = sameGridMC.size(); j < p; ++j) {
            if (_MCT2Num.contains(sameGridMC[j])) {
                if (n < _MCT2Num[sameGridMC[j]]) {
                    d += sameGridDemand[j];
                }
            }
        }
        for (int j = 0, p = sameGridMC.size(); j < p; ++j) {
            int k = adjHGridDemand[j];
            if (_c1) {
                if (_c1->_MCT2Num.contains(adjHGridMC[j])) {
                    if (n < _c1->_MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        _c1->_grids[i]->incSupply(k);
                    }
                }
            }
            if (_c2) {
                if (_c2->_MCT2Num.contains(adjHGridMC[j])) {
                    if (n < _c2->_MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        _c2->_grids[i]->incSupply(k);
                    }
                }
            }
        }
        _grids[i]->incSupply(d);
    }
}

Grid& Coordinate::getGrid(size_t i) {
    return *_grids[i];
}

int Coordinate::getRow() const {
    return _row;
}

int Coordinate::getColumn() const {
    return _column;
}

// Grid
Grid::Grid(int supply, Layer& layer)
    : _supply(supply), _layer(layer), _coordinate(nullptr) {}

Grid::Grid(Grid& a)
    : _supply(a._supply),
      _layer(a._layer),
      _coordinate(a._coordinate),
      _nets(safe::unordered_map<unsigned, GridNet*>()) {}

void Grid::assignCoordinate(Coordinate* c) {
    _coordinate = c;
}

void Grid::incSupply(int d) {
    _supply += d;
}

void Grid::decSupply(int d) {
    _supply -= d;
}

void Grid::addNet(GridNet& net) {
    // if (_nets.find(net.getId()) == _nets.end()) {
    if (!_nets.contains(net.getId())) {
        _nets[net.getId()] = &net;
        _supply -= 1;
    }
}

bool Grid::canGetNet(const GridNet& net) const {
    // return _nets.find(net.getId()) != _nets.end();
    return _nets.contains(net.getId());
}

bool Grid::canGetNet(unsigned i) const {
    return _nets.contains(i);
}

GridNet& Grid::getNet(unsigned i) {
    // if (_nets.find(i) == _nets.end()) {
    assert(_nets.contains(i));
    return *_nets[i];
}

void Grid::rmNet(GridNet& net) {
    assert(_nets.contains(net.getId()));
    _nets.erase(net.getId());
}

void Grid::rmNet(unsigned i) {
    assert(_nets.contains(i));
    _nets.erase(i);
}

int Grid::getRow() const {
    return _coordinate->getRow();
}

int Grid::getColumn() const {
    return _coordinate->getColumn();
}

int Grid::getSupply() const {
    return _supply;
}

int Grid::getLayer() const {
    return _layer.getLayerIdx();
}
