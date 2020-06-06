/***********************************************************************

  FileName    [PrimeMan.cpp]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file describes the functions in "PrimeMan.h".

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "PrimeMan.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

PrimeMan::PrimeMan(std::fstream& input) {
    readFile(input);
}

void PrimeMan::readFile(std::fstream& input) {
    std::string str;
    std::string buf;

    /*MaxCellMove <maxMoveCount>*/
    input >> str;  // MaxCellMove
    assert(str == "MaxCellMove");
    input >> _maxMove;  // <maxMoveCount>

    /*GGridBoundaryIdx <rowBeginIdx> <colBeginIdx> <rowEndIdx> <colEndIdx>*/
    input >> str;  // GGridBoundaryIdx
    assert(str == "GGridBoundaryIdx");
    int rb, cb, re, ce;
    input >> rb >> cb >> re >>
        ce;  //<rowBeginIdx> <colBeginIdx> <rowEndIdx> <colEndIdx>
    _rowBase = cb;
    _columnBase = rb;
    _rowRange = re - rb + 1;
    _columnRange = ce - cb + 1;

    /*NumLayers <LayerCount>
      Lay <LayerName> <Idx> <RoutingDirection> <defaultSupplyOfOneGGrid>*/
    input >> str;  // NumLayers
    assert(str == "NumLayer");
    input >> _layer;  //<LayerCount>
    _layers.reserve(_layer);
    for (int i = 0; i < _layer; ++i) {
        _layers.push_back(0);
    }
    constructCoordinate();
    for (int i = 0; i < _layer; ++i) {
        input >> str;  // Lay
        assert(str == "Lay");
        input >> str;  //<LayerName>
        int idx;
        input >> idx;  //<Idx>
        assert(!_Layer2Idx.contains(str));
        _Layer2Idx[str] = idx;
        bool direction = false;
        input >> buf;  //<RoutingDirection>
        if (buf == "H") {
            direction = false;
        } else {
            assert(buf == "V");
            direction = true;
        }
        // } else if (buf == "V") {
        //     direction = true;
        // } else {
        //     assert(buf == "H" || buf == "V");
        // }
        int supply;
        input >> supply;  //<defaultSupplyOfOneGGrid>
        Layer* l = new Layer(str, idx - 1, direction, supply, _area);
        _layers[idx - 1] = l;
    }
    connectCoordinateGrid();

    /*NumNonDefaultSupplyGGrid <nonDefaultSupplyGGridCount>
      <rowIdx> <colIdx> <LayIdx> <incrOrDecrValue>*/
    input >> str;  // NumNonDefaultSupplyGGrid
    assert(str == "NumNonDefaultSupplyGGrid");
    int count, row, column, layer, val;
    input >> count;
    for (int i = 0; i < count; ++i) {
        input >> row >> column >> layer >> val;
        _layers[layer - 1]
            ->getGrid(getIdx(row - _rowBase, column - _columnBase))
            .incSupply(val);
    }

    /*NumMasterCell <masterCellCount>
      MasterCell <masterCellName> <pinCount> <blockageCount>
      Pin <pinName> <pinLayer>
      Blkg <blockageName> <blockageLayer> <demand>
    */
    int demand;
    input >> str;  // NumMasterCell
    assert(str == "NumMasterCell");
    input >> count;  //<masterCellCount>
    _MasterCells.reserve(count);
    for (int i = 0; i < count; ++i) {
        input >> str;  // MasterCell
        assert(str == "MasterCell");
        input >> str;  // <masterCellName>
        // assert(_MasterCell2Idx.count(str) == 0);
        assert(!_MasterCell2Idx.contains(str));
        _MasterCell2Idx[str] = i;
        MasterCellType* mct = new MasterCellType(str, i, _layer);
        _MasterCells.push_back(mct);
        int pin, blockage;
        input >> pin >> blockage;  // <pinCount> <blockageCount>
        for (int j = 0; j < pin; ++j) {
            input >> str;  // Pin
            assert(str == "Pin");
            input >> str >> buf;  // <pinName> <pinLayer>
            assert(_Layer2Idx.contains(buf));
            mct->AddPin(str, _Layer2Idx[buf]);
        }
        for (int j = 0; j < blockage; ++j) {
            input >> str;  // Blkg
            assert(str == "Blkg");
            input >> str >> buf >>
                demand;  // <blockageName> <blockageLayer> <demand>
            assert(_Layer2Idx.contains(buf));
            mct->AddBlkg(str, _Layer2Idx[buf], demand);
        }
    }

    /*NumNeighborCellExtraDemand <count>
      sameGGrid <masterCellName1> <masterCellName2> <layerName> <demand>
      adjHGGrid <masterCellName1> <masterCellName2> <layerName> <demand>*/
    int mc1, mc2;
    input >> str;  // NumNeighborCellExtraDemand
    assert(str == "NumNeighborCellExtraDemand");
    input >> count;  //<count>
    for (int i = 0; i < count; ++i) {
        input >> str;  // sameGGrid || adjHGGrid
        input >> buf;  // <masterCellName1>
        mc1 = _MasterCell2Idx[buf];
        input >> buf;  // <masterCellName2>
        mc2 = _MasterCell2Idx[buf];
        input >> buf >> demand;  // <layerName> <demand>
        layer = _Layer2Idx[buf];
        if (str == "sameGGrid") {
            _MasterCells[mc1]->AddExtraSame(mc2, demand, layer);
            _MasterCells[mc2]->AddExtraSame(mc1, demand, layer);
        } else {
            assert(str == "adjHGGrid");
            _MasterCells[mc1]->AddExtraadjH(mc2, demand, layer);
            _MasterCells[mc2]->AddExtraadjH(mc1, demand, layer);
        }
        // } else if (str == "adjHGGrid") {
        //     _MasterCells[mc1]->AddExtraadjH(mc2, demand, layer);
        //     _MasterCells[mc2]->AddExtraadjH(mc1, demand, layer);
        // } else {
        //     assert(str == "sameGGrid" || str == "adjHGGrid");
        // }
    }

    /*NumCellInst <cellInstCount>
      CellInst <instName> <masterCellName> <gGridRowIdx> <gGridColIdx>
      <movableCstr>*/
    input >> str;  // NumCellInst
    assert(str == "NumCellInst");
    input >> count;  // <cellInstCount>
    _cells.reserve(count);
    for (int i = 0; i < count; ++i) {
        input >> str;  // CellInst
        assert(str == "CellInst");
        input >> str;  // <instName>
        // assert(_Cell2Idx.count(str) == 0);
        assert(!_Cell2Idx.contains(str));
        _Cell2Idx[str] = i;
        input >> buf;  // <masterCellName>
        MasterCellType& MCT = *_MasterCells[_MasterCell2Idx[buf]];
        input >> row >> column >>
            buf;  // <gGridRowIdx> <gGridColIdx> <movableCstr>
        bool movable = false;
        if (buf == "Movable") {
            movable = true;
        } else {
            assert(buf == "Fixed");
            movable = false;
        }
        // } else if (buf == "Fixed") {
        //     movable = false;
        // } else {
        //     assert(buf == "Fixed" || buf == "Movable");
        // }
        Cell* cell = new Cell(str, MCT, movable, i);
        _cells.push_back(cell);
        int rIdx = row - _rowBase, cIdx = column - _columnBase;
        cell->setCoordinate(rIdx, cIdx);
        Coordinate* c = _coordinates[getIdx(rIdx, cIdx)];
        int left, right;
        left = getLeft(rIdx, cIdx);
        right = getRight(rIdx, cIdx);
        Coordinate* c1 = 0;
        Coordinate* c2 = 0;
        if (left > -1) {
            c1 = _coordinates[left];
        }
        if (right > -1) {
            c2 = _coordinates[right];
        }
        c->addCell(cell, c1, c2);
    }

    /*NumNets <netCount>
      Net <netName> <numPins> <minRoutingLayConstraint>
      Pin <instName>/<masterPinName>*/
    input >> str;  // NumNets
    assert(str == "NumNets");
    input >> count;  // <netCount>
    for (int i = 0; i < count; ++i) {
        int numPins;
        input >> str;  // Net
        assert(str == "Net");
        input >> str;      // <netName>
        input >> numPins;  // <numPins>
        // assert(_Net2Idx.count(str) == 0);
        assert(!_Net2Idx.contains(str));
        _Net2Idx[str] = i;
        int minLay;
        input >> str; // <minRoutingLayConstraint>
        if(str == "NoCstr") {
            minLay = 0;
        } else {
            assert(_Layer2Idx.contains(str));
            minLay = _Layer2Idx[str];
        }
        Net* net = new Net(str, i, numPins, minLay);
        _nets.push_back(net);
        std::string inst, masterPin;
        std::string delimiter = "/";
        for (int j = 0; j < numPins; ++j) {
            input >> str;  // Pin
            assert(str == "Pin");
            input >> str;  // <instName>/<masterPinName>
            size_t pos = str.find(delimiter);
            inst = str.substr(0, pos);
            pos++;
            masterPin = str.substr(pos, str.size() - pos);
            // assert(_Cell2Idx.count(inst) == 1);
            assert(_Cell2Idx.contains(inst));
            Cell* cell = _cells[_Cell2Idx[inst]];
            Pin& pin = cell->getPin(masterPin);
            net->addPin(&pin);
        }
    }

    /*NumRoutes <routeSegmentCount>
      <sRowIdx> <sColIdx> <sLayIdx> <eRowIdx> <eColIdx> <eLayIdx> <netName>*/
    int srow, scol, slay, erow, ecol, elay;
    input >> str;  // NumRoutes
    assert(str == "NumRoutes");
    input >> count;  // <routeSegmentCount>
    for (int i = 0; i < count; ++i) {
        input >> srow >> scol >> slay >> erow >> ecol >> elay >>
            str;  // <sRowIdx> <sColIdx> <sLayIdx> <eRowIdx> <eColIdx> <eLayIdx>
                  // <netName>
        // assert(_Net2Idx.count(str) == 1);
        // assert(_Net2Idx.contains(str));
        Net* net = _nets[_Net2Idx.at(str)];
        assignRoute(srow - _rowBase, scol - _columnBase, slay - 1,
                    erow - _rowBase, ecol - _columnBase, elay - 1, net);
    }
}

PrimeMan::~PrimeMan() {
    for (int i = 0, n = _layers.size(); i < n; ++i) {
        delete _layers[i];
    }
    for (int i = 0, n = _coordinates.size(); i < n; ++i) {
        delete _coordinates[i];
    }
    for (int i = 0, n = _MasterCells.size(); i < n; ++i) {
        delete _MasterCells[i];
    }
    for (int i = 0, n = _cells.size(); i < n; ++i) {
        delete _cells[i];
    }
    for (int i = 0, n = _nets.size(); i < n; ++i) {
        delete _nets[i];
    }
}

int PrimeMan::getIdx(int row, int column) const {
    assert(column >= 0 && column < _columnRange && row >= 0 && row < _rowRange);
    return column * _rowRange + row;
}

int PrimeMan::getLeft(int row, int column) const {
    return (column == 0) ? -1 : getIdx(row, column - 1);
}

int PrimeMan::getRight(int row, int column) const {
    return (column == _columnRange - 1) ? -1 : getIdx(row, column + 1);
}

int PrimeMan::getDown(int row, int column) const {
    return (row == 0) ? -1 : getIdx(row - 1, column);
}

int PrimeMan::getUp(int row, int column) const {
    return (row == _rowRange - 1) ? -1 : getIdx(row + 1, column);
}

Layer& PrimeMan::getLayer(int layer) {
    return *_layers[layer];
}

Coordinate& PrimeMan::getCoordinate(unsigned i) {
    return *_coordinates[i];
}

Cell& PrimeMan::getCell(unsigned i) {
    return *_cells[i];
}

Net& PrimeMan::getNet(unsigned i) {
    return *_nets[i];
}

void PrimeMan::constructCoordinate() {
    _area = _columnRange * _rowRange;
    _coordinates.reserve(_area);
    for (int i = 0; i < _columnRange; ++i) {
        for (int j = 0; j < _rowRange; ++j) {
            Coordinate* c = new Coordinate(j, i, _layer);
            _coordinates.push_back(c);
        }
    }
}

void PrimeMan::connectCoordinateGrid() {
    for (int i = 0; i < _area; ++i) {
        Coordinate* c = _coordinates[i];
        for (int j = 0; j < _layer; ++j) {
            Layer* l = _layers[j];
            Grid& g = l->getGrid(i);
            g.assignCoordinate(c);
            c->addGrid(&g);
        }
    }
}

void PrimeMan::assignRoute(int srow,
                           int scol,
                           int slay,
                           int erow,
                           int ecol,
                           int elay,
                           Net* net) {
    for (int i = slay; i <= elay; ++i) {
        Layer* l = _layers[i];
        for (int j = scol; j <= ecol; ++j) {
            for (int k = srow; k <= erow; ++k) {
                Grid& g = l->getGrid(getIdx(k, j));
                g.addNet(*net);
            }
        }
    }
}
