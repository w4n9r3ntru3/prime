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
Layer::Layer(unsigned idx, bool d, int supply, unsigned area)
    : _idx(idx), _direction(d) {
    _grids.reserve(area);
    for (unsigned i = 0; i < area; ++i) {
        _grids.push_back(Grid(supply));
    }
}

// Layer::Layer(const Layer& l)
//     : _idx(l._idx),
//       _direction(l._direction),
//       _grids(l._grids) {}

Layer::Layer(Layer&& l)
    : _idx(l._idx), _direction(l._direction), _grids(std::move(l._grids)) {}

Layer& Layer::operator=(Layer&& l) {
    _idx = l._idx;
    _direction = l._direction;
    _grids = std::move(l._grids);
    return *this;
}

unsigned Layer::getLayerIdx() const {
    return _idx;
}

bool Layer::getDirection() const {
    return _direction;
}

Grid& Layer::getGrid(unsigned idx) {
    return _grids[idx];
}

// Coordinate
Coordinate::Coordinate(unsigned x,
                       unsigned y,
                       unsigned idx,
                       int left,
                       int right)
    : _row(x), _column(y), _idx(idx), _c1(left), _c2(right) {}

// Coordinate::Coordinate(const Coordinate& c) : _row(c._row),
// _column(c._column), _idx(c._idx) {}

Coordinate::Coordinate(Coordinate&& c)
    : _row(c._row),
      _column(c._column),
      _idx(c._idx),
      _c1(c._c1),
      _c2(c._c2),
      _MCT2Num(std::move(c._MCT2Num)) {}

Coordinate& Coordinate::operator=(Coordinate&& c) {
    _row = c._row;
    _column = c._column;
    _idx = c._idx;
    _c1 = c._c1;
    _c2 = c._c2;
    _MCT2Num = std::move(c._MCT2Num);
    return *this;
}

bool Coordinate::CanAddCell(Cell& cell,
                            safe::vector<Coordinate>& coordinates,
                            safe::vector<Layer>& layers) const {
    unsigned id = cell.getMasterCellId();
    unsigned n;
    if (_MCT2Num.contains(id)) {
        assert(_MCT2Num.at(id) >= 1);
        n = _MCT2Num.at(id) + 1;
    } else {
        n = 1;
    }
    for (unsigned i = 0; i < layers.size(); ++i) {
        Layer& layer = layers[i];
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
            if (_c1 >= 0) {
                Coordinate& c_1 = coordinates[_c1];
                if (c_1._MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= c_1._MCT2Num.find(adjHGridMC[j])->second) {
                        if (layer.getGrid(c_1._idx).getSupply() < k) {
                            return false;
                        }
                        d += k;
                    }
                }
            }
            if (_c2 >= 0) {
                Coordinate& c_2 = coordinates[_c2];
                if (c_2._MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= c_2._MCT2Num.find(adjHGridMC[j])->second) {
                        if (layer.getGrid(c_2._idx).getSupply() < k) {
                            return false;
                        }
                        d += k;
                    }
                }
            }
        }
        if (layer.getGrid(_idx).getSupply() < d) {
            return false;
        }
    }
    return true;
}

void Coordinate::initCell(Cell& cell,
                         safe::vector<Coordinate>& coordinates,
                         safe::vector<Layer>& layers) {
    unsigned id = cell.getMasterCellId();
    unsigned n;
    if (_MCT2Num.contains(id)) {
        assert(_MCT2Num[id] >= 1);
        n = ++_MCT2Num[id];
    } else {
        n = 1;
        _MCT2Num[id] = 1;
    }
    for (int i = 0, m = layers.size(); i < m; ++i) {
        Layer& layer = layers[i];
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
        for (int j = 0, p = adjHGridMC.size(); j < p; ++j) {
            int k = adjHGridDemand[j];
            if (_c1 >= 0) {
                Coordinate& c_1 = coordinates[_c1];
                if (c_1._MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= c_1._MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        layer.getGrid(c_1._idx).decSupply(k);
                    }
                }
            }
            if (_c2 >= 0) {
                Coordinate& c_2 = coordinates[_c2];
                if (c_2._MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= c_2._MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        layer.getGrid(c_2._idx).decSupply(k);
                    }
                }
            }
        }
        layer.getGrid(_idx).decSupply(d);
    }
}

void Coordinate::addCell(Cell& cell,
                         safe::vector<Coordinate>& coordinates,
                         safe::vector<Layer>& layers,
                         safe::vector<Pin>& pins) {
    unsigned id = cell.getMasterCellId();
    unsigned n;
    if (_MCT2Num.contains(id)) {
        assert(_MCT2Num[id] >= 1);
        n = ++_MCT2Num[id];
    } else {
        n = 1;
        _MCT2Num[id] = 1;
    }
    for (unsigned i = 0, m = layers.size(); i < m; ++i) {
        Layer& layer = layers[i];
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
        for (int j = 0, p = adjHGridMC.size(); j < p; ++j) {
            int k = adjHGridDemand[j];
            if (_c1 >= 0) {
                Coordinate& c_1 = coordinates[_c1];
                if (c_1._MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= c_1._MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        layer.getGrid(c_1._idx).decSupply(k);
                    }
                }
            }
            if (_c2 >= 0) {
                Coordinate& c_2 = coordinates[_c2];
                if (c_2._MCT2Num.contains(adjHGridMC[j])) {
                    if (n <= c_2._MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        layer.getGrid(c_2._idx).decSupply(k);
                    }
                }
            }
        }
        Grid& grid = layer.getGrid(_idx);
        grid.decSupply(d);
        grid.addPin(cell.getPinLayer(i),pins);
    }
}

void Coordinate::rmCell(Cell& cell,
                        safe::vector<Coordinate>& coordinates,
                        safe::vector<Layer>& layers,
                        safe::vector<Pin>& pins) {
    unsigned id = cell.getMasterCellId();
    assert(_MCT2Num.contains(id));
    unsigned n = --_MCT2Num[id];
    if (n == 0) {
        _MCT2Num.erase(id);
    }
    for (unsigned i = 0, m = layers.size(); i < m; ++i) {
        Layer& layer = layers[i];
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
        for (int j = 0, p = adjHGridMC.size(); j < p; ++j) {
            int k = adjHGridDemand[j];
            if (_c1 >= 0) {
                Coordinate& c_1 = coordinates[_c1];
                if (c_1._MCT2Num.contains(adjHGridMC[j])) {
                    if (n < c_1._MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        layer.getGrid(c_1._idx).incSupply(k);
                    }
                }
            }
            if (_c2 >= 0) {
                Coordinate& c_2 = coordinates[_c2];
                if (c_2._MCT2Num.contains(adjHGridMC[j])) {
                    if (n < c_2._MCT2Num[adjHGridMC[j]]) {
                        d += k;
                        layer.getGrid(c_2._idx).incSupply(k);
                    }
                }
            }
        }
        Grid& grid = layer.getGrid(_idx);
        grid.incSupply(d);
        grid.rmPin(cell.getPinLayer(i),pins);
    }
}

int Coordinate::getRow() const {
    return _row;
}

int Coordinate::getColumn() const {
    return _column;
}

// Grid
Grid::Grid(int supply) : _supply(supply) {}

// Grid::Grid(const Grid& g)
//     : _supply(g._supply),
//       _nets(g._nets) {}

Grid::Grid(Grid&& g) : _supply(g._supply), _nets(std::move(g._nets)) {}

Grid& Grid::operator=(Grid&& g) {
    _supply = g._supply;
    _nets = std::move(g._nets);
    return *this;
}

void Grid::incSupply(int d) {
    _supply += d;
}

void Grid::decSupply(int d) {
    _supply -= d;
}

void Grid::addPin(Pin& pin) {
    if (pin.get_net_idx() == -1) {
        return;
    }
    assert(pin.get_net_idx() >= 0);
    if (!_pins.contains(pin.get_net_idx())) {
        _pins[pin.get_net_idx()] = 1;
        _supply -= 1;
    } else {
        _pins[pin.get_net_idx()] += 1;
    }
}

void Grid::addPin(int i) {
    if (i == -1) {
        return;
    }
    assert(i >= 0);
    if (!_pins.contains(i)) {
        _pins[i] = 1;
        _supply -= 1;
    } else {
        _pins[i] += 1;
    }
}

void Grid::addPin(const safe::vector<unsigned>& idx, safe::vector<Pin>& pins) {
    for (unsigned i = 0; i < idx.size(); ++i) {
        addPin(pins[idx[i]]);
    }
}

void Grid::rmPin(Pin& pin) {
    if (pin.get_net_idx() == -1) {
        return;
    }
    assert(pin.get_net_idx() >= 0);
    assert(_pins.contains(pin.get_net_idx()));
    _pins[pin.get_net_idx()] -= 1;
    if (_pins[pin.get_net_idx()] == 0) {
        _pins.erase(pin.get_net_idx());
        if (!_nets.contains(pin.get_net_idx())) {
            _supply += 1;
        }
    }
}

void Grid::rmPin(int i) {
    if (i == -1) {
        return;
    }
    assert(i >= 0);
    assert(_pins.contains(i));
    _pins[i] -= 1;
    if (_pins[i] == 0) {
        _pins.erase(i);
        if (!_nets.contains(i)) {
            _supply += 1;
        }
    }
}

void Grid::rmPin(const safe::vector<unsigned>& idx, safe::vector<Pin>& pins) {
    for (unsigned i = 0; i < idx.size(); ++i) {
        rmPin(pins[idx[i]]);
    }
}

void Grid::addNet(GridNet& net) {
    // if (_nets.find(net.getId()) == _nets.end()) {
    if (!_nets.contains(net.getIdx())) {
        _nets.insert(net.getIdx());
        if (!_pins.contains(net.getIdx())) {
            _supply -= 1;
        }
    }
}

void Grid::addNet(unsigned i) {
    if (!_nets.contains(i)) {
        _nets.insert(i);
        if (!_pins.contains(i)) {
            _supply -= 1;
        }
    }
}

GridNet& Grid::getNet(unsigned i, std::vector<GridNet>& nets) {
    // if (_nets.find(i) == _nets.end()) {
    assert(_nets.contains(i));
    return nets[i];
}

void Grid::rmNet(GridNet& net) {
    assert(_nets.contains(net.getIdx()));
    _nets.erase(net.getIdx());
}

void Grid::rmNet(unsigned i) {
    assert(_nets.contains(i));
    _nets.erase(i);
}

int Grid::getSupply() const {
    return _supply;
}

bool Grid::canGetNet(const GridNet& net) const {
    // return _nets.find(net.getId()) != _nets.end();
    return _nets.contains(net.getIdx()) || _pins.contains(net.getIdx());
}

bool Grid::canGetNet(unsigned i) const {
    return _nets.contains(i) || _pins.contains(i);
}

bool Grid::routable(const GridNet& net) const {
    if (canGetNet(net)) {
        return true;
    }
    assert(_supply >= 0);
    return _supply > 0;
}