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

#include <iostream>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

// Layer
Layer::Layer(const std::string name, int i, bool d, int supply, int area)
    : _LayerName(name), _idx(i), _direction(d) {
    _grids.reserve(area);
    for (int i = 0; i < area; ++i) {
        Grid* g = new Grid(supply, *this);
        _grids.push_back(g);
    }
}

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

bool Coordinate::CanAddCell(Cell& cell) {
    for (int i = 0, n = _grids.size(); i < n; ++i) {
        if (!_grids[i]->CanAddCell(cell.getMasterCell())) {
            return false;
        }
    }
    return true;
}

void Coordinate::addCell(Cell& cell) {
    // for every layer
    for (int i = 0, n = _grids.size(); i < n; ++i) {
        _grids[i]->addCell(cell.getMasterCell());
        safe::vector<unsigned>& adjHMC = cell.getadjHGridMC(i);
        safe::vector<unsigned>& SameMC = cell.getSameGridMC(i);
        safe::vector<int>& adjHDemand = cell.getadjHGridDemand(i);
        safe::vector<int>& SameDemand = cell.getSameGridDemand(i);
        addConstraint(i, SameMC, SameDemand);
        if (_c1) {
            _c1->addConstraint(i, adjHMC, adjHDemand);
        }
        if (_c2) {
            _c2->addConstraint(i, adjHMC, adjHDemand);
        }
    }
}

void Coordinate::addConstraint(int layer,
                               safe::vector<unsigned>& mc,
                               safe::vector<int>& demand) {
    assert(mc.size() == demand.size());
    for (int i = 0, n = mc.size(); i < n; ++i) {
        _grids[layer]->addConstraint(mc[i], demand[i]);
    }
}

void Coordinate::moveCell(Cell& cell) {
    for (int i = 0, n = _grids.size(); i < n; ++i) {
        _grids[i]->moveCell(cell.getMasterCell());
        safe::vector<unsigned>& adjHMC = cell.getadjHGridMC(i);
        safe::vector<unsigned>& SameMC = cell.getSameGridMC(i);
        safe::vector<int>& adjHDemand = cell.getadjHGridDemand(i);
        safe::vector<int>& SameDemand = cell.getSameGridDemand(i);
        moveConstraint(i, SameMC, SameDemand);
        if (_c1) {
            _c1->moveConstraint(i, adjHMC, adjHDemand);
        }
        if (_c2) {
            _c2->moveConstraint(i, adjHMC, adjHDemand);
        }
    }
}

void Coordinate::moveConstraint(int layer,
                                safe::vector<unsigned>& mc,
                                safe::vector<int>& demand) {
    assert(mc.size() == demand.size());
    for (int i = 0, n = mc.size(); i < n; ++i) {
        _grids[layer]->moveConstraint(mc[i], demand[i]);
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
    : _supply(a._supply), _layer(a._layer), _coordinate(a._coordinate) {}

void Grid::assignCoordinate(Coordinate* c) {
    _coordinate = c;
}

void Grid::incSupply(int d) {
    _supply += d;
}

void Grid::decSupply(int d) {
    _supply -= d;
}

void Grid::addConstraint(unsigned mc, int demand) {
    if (_Cell2Demand.contains(mc)) {
        _Cell2Demand[mc] += demand;
    } else {
        _Cell2Demand[mc] = demand;
    }
}

void Grid::moveConstraint(unsigned mc, int demand) {
    demand = _Cell2Demand.at(mc) - demand;
    assert(demand >= 0);
    if (demand > 0) {
        _Cell2Demand[mc] = demand;
    } else {
        _Cell2Demand.erase(mc);
    }
}

bool Grid::CanAddCell(MasterCellType& mct) {
    // FIXME inconsistency?
    int demand = 0;
    demand += mct.getLayerDemand(_layer.getLayerIdx());
    // https://stackoverflow.com/a/5683030
    return !(getDemand(mct.getId(), demand) && demand > _supply);
}

void Grid::addCell(MasterCellType& mct) {
    int demand = 0;
    _supply -= mct.getLayerDemand(_layer.getLayerIdx());
    if (getDemand(mct.getId(), demand)) {
        _supply -= demand;
    }
}

void Grid::moveCell(MasterCellType& mct) {
    int demand = 0;
    _supply += mct.getLayerDemand(_layer.getLayerIdx());
    if (getDemand(mct.getId(), demand)) {
        _supply += demand;
    }
}

void Grid::addNet(GridNet& net) {
    // if (_nets.find(net.getId()) == _nets.end()) {
    if (!_nets.contains(net.getId())) {
        _nets[net.getId()] = &net;
        _supply -= 1;
    }
}

bool Grid::getNet(GridNet& net) {
    // return _nets.find(net.getId()) != _nets.end();
    return _nets.contains(net.getId());
}

GridNet* Grid::getNet(unsigned i) {
    // if (_nets.find(i) == _nets.end()) {
    if (!_nets.contains(i)) {
        return nullptr;
    }
    return _nets[i];
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

bool Grid::getDemand(unsigned mc, int& demand) {
    // if (_Cell2Demand.find(mc) == _Cell2Demand.end()) {
    if (!_Cell2Demand.contains(mc)) {
        return false;
    }
    demand += _Cell2Demand[mc];
    return true;
}
