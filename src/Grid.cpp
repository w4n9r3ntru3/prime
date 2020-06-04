/***********************************************************************

  FileName    [Grid.cpp]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file describes the Layers, the Grids, and their Coordinates(to
  save cells).

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Grid.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

//Layer
Layer::Layer(const std::string name, int i, bool d, int supply, int area)
    : _direction(d), _LayerName(name), _idx(i) {
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

int Layer::getLayerIdx() const {\
    return _idx;
}

Grid& Layer::getGrid(int i) {
    return *_grids[i];
}

//Coordinate
Coordinate::Coordinate(int x, int y, int layer) : _row(x), _column(y) {
    _grids.reserve(layer);
}

void Coordinate::addGrid(Grid* g) {
    _grids.push_back(g);
}

void Coordinate::addCell(Cell* cell, Coordinate* c1, Coordinate* c2) {
    // for every layer
    for (int i = 0, n = _grids.size(); i < n; ++i) {
        _grids[i]->addCell(cell->getMasterCellId());
        safe::vector<unsigned> adjHMC = cell->getadjHGridMC(i);
        safe::vector<unsigned> SameMC = cell->getSameGridMC(i);
        safe::vector<int> adjHDemand = cell->getadjHGridDemand(i);
        safe::vector<int> SameDemand = cell->getSameGridDemand(i);
        addConstraint(i, SameMC, SameDemand);
        if (c1) {
            c1->addConstraint(i, adjHMC, adjHDemand);
        }
        if (c2) {
            c2->addConstraint(i, adjHMC, adjHDemand);
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

Grid& Coordinate::getGrid(size_t i) {
    return *_grids[i];
}

int Coordinate::getRow() const {
    return _row;
}

int Coordinate::getColumn() const {
    return _column;
}

//Grid
Grid::Grid(int supply, Layer& layer) : _layer(layer), _supply(supply) {
    
}

Grid::Grid(Grid& a)
        : _layer(a._layer), _supply(a._supply), _coordinate(a._coordinate) {

}

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
    // if (_Cell2Demand.find(mc) == _Cell2Demand.end()) {
    if (contains(_Cell2Demand, mc)) {
        _Cell2Demand[mc] = demand;
    } else {
        _Cell2Demand[mc] += demand;
    }
}

void Grid::moveConstraint(unsigned mc, int demand) {
    // assert(_Cell2Demand.count(mc) > 0);
    assert(contains(_Cell2Demand, mc));
    demand = _Cell2Demand[mc] - demand;
    if (demand > 0) {
        _Cell2Demand[mc] = demand;
    } else {
        _Cell2Demand.erase(mc);
    }
}

void Grid::addCell(unsigned mc) {
    int demand;
    if (getDemand(mc, demand)) {
        _supply -= demand;
    }
}

void Grid::addNet(Net& net) {
    // if (_nets.find(net.getId()) == _nets.end()) {
    if (!contains(_nets, net.getId())) {
        _nets[net.getId()] = &net;
        _supply -= 1;
    }
}

bool Grid::getNet(Net& net) {
    // return _nets.find(net.getId()) != _nets.end();
    return contains(_nets, net.getId());
}

Net* Grid::getNet(unsigned i) {
    // if (_nets.find(i) == _nets.end()) {
    if (!contains(_nets, i)) {
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
    if (contains(_Cell2Demand, mc)) {
        return false;
    } else {
        demand = _Cell2Demand[mc];
    }
    return true;
}