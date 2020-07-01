/***********************************************************************

  FileName    [Chip.cpp]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file describes the functions in "Chip.h".

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Chip.h"

#include <assert.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

Chip::Chip(std::fstream& input) {
    readFile(input);
}

void Chip::readFile(std::fstream& input) {
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
        _Layer2Idx[str] = idx - 1;
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
        _layers[idx - 1].reset(l);
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
        MasterCellType mct = MasterCellType(str, i, _layer);
        int pin, blockage;
        input >> pin >> blockage;  // <pinCount> <blockageCount>
        for (int j = 0; j < pin; ++j) {
            input >> str;  // Pin
            assert(str == "Pin");
            input >> str >> buf;  // <pinName> <pinLayer>
            assert(_Layer2Idx.contains(buf));
            mct.AddPin(str, _Layer2Idx[buf]);
        }
        for (int j = 0; j < blockage; ++j) {
            input >> str;  // Blkg
            assert(str == "Blkg");
            input >> str >> buf >>
                demand;  // <blockageName> <blockageLayer> <demand>
            assert(_Layer2Idx.contains(buf));
            mct.AddBlkg(str, _Layer2Idx[buf], demand);
        }
        _MasterCells.push_back(std::move(mct));
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
        // FIXME the original version does not check, intentional?
        mc1 = _MasterCell2Idx.at(buf);
        input >> buf;  // <masterCellName2>
        // FIXME the original version does not check, intentional?
        mc2 = _MasterCell2Idx.at(buf);
        input >> buf >> demand;  // <layerName> <demand>
        layer = _Layer2Idx[buf];
        if (str == "sameGGrid") {
            _MasterCells[mc1].AddExtraSame(mc2, demand, layer);
            _MasterCells[mc2].AddExtraSame(mc1, demand, layer);
        } else {
            assert(str == "adjHGGrid");
            _MasterCells[mc1].AddExtraadjH(mc2, demand, layer);
            if (mc1 != mc2) {
                _MasterCells[mc2].AddExtraadjH(mc1, demand, layer);
            }
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
        _cells.push_back(0);
    }
    for (int i = 0; i < count; ++i) {
        input >> str;  // CellInst
        assert(str == "CellInst");
        input >> str;  // <instName>
        // assert(_Cell2Idx.count(str) == 0);
        assert(!_Cell2Idx.contains(str));
        _Cell2Idx[str] = i;
        input >> buf;  // <masterCellName>
        // FIXME the original version does not check, intentional?
        MasterCellType& MCT = _MasterCells[_MasterCell2Idx.at(buf)];
        input >> row >> column >> buf;
        // <gGridRowIdx> <gGridColIdx> <movableCstr>
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
        int rIdx = row - _rowBase, cIdx = column - _columnBase;
        cell->setCoordinate(rIdx, cIdx);
        _coordinates[getIdx(rIdx, cIdx)]->addCell(*cell);
        _cells[i].reset(cell);
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
        input >> buf;  // <minRoutingLayConstraint>
        if (buf == "NoCstr") {
            minLay = 0;
        } else {
            assert(_Layer2Idx.contains(buf));
            minLay = _Layer2Idx[buf];
        }

        _grid_nets.push_back(GridNet(str, i, numPins, minLay));
        // TODO:
        // _quad_tree_nets.push_back(QuadTree(str, i, minLay, _rowRange, _columnRange));

        // ! substituted
        // TreeNet tree_net = TreeNet(std::move(str), i, numPins, minLay);
        // _tree_nets.push_back(std::move(tree_net));

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
            // assert(_Cell2Idx.contains(inst));
            Pin& pin = _cells[_Cell2Idx.at(inst)]->getPin(masterPin);
            // Cell& cell_ = pin.get_cell();
            // std::cout << &cell << " " << &cell_ << std::endl;
            _grid_nets[i].addPin(std::shared_ptr<Pin>(&pin));
            pin.setNet(std::shared_ptr<GridNet>(&_grid_nets[i]));
            Grid& g = _layers[pin.getLayer()]->getGrid(
                getIdx(pin.getRow(), pin.getColumn()));
            g.addNet(_grid_nets[i]);
            // TODO: add pin to quad tree
            // _quad_tree_nets[i].add_pin(&pin);
        }
    }

    // safe::unordered_map<std::string, std::vector<Segment>> segments;

    /*NumRoutes <routeSegmentCount>
      <sRowIdx> <sColIdx> <sLayIdx> <eRowIdx> <eColIdx> <eLayIdx> <netName>*/
    int srow, scol, slay, erow, ecol, elay;
    input >> str;  // NumRoutes
    assert(str == "NumRoutes");
    input >> count;  // <routeSegmentCount>
    for (int i = 0; i < count; ++i) {
        input >> srow >> scol >> slay >> erow >> ecol >> elay >> str;
        // <sRowIdx> <sColIdx> <sLayIdx> <eRowIdx> <eColIdx> <eLayIdx> <netName>

        // assert(_Net2Idx.count(str) == 1);
        // assert(_Net2Idx.contains(str));
        assert(slay >= 1 && elay >= 1);

        // TODO: change to tree-based net data structure
        GridNet& net = _grid_nets[_Net2Idx.at(str)];
        net.addSegment(srow, scol, slay, erow, ecol, elay);
        // assignRoute(srow - _rowBase, scol - _columnBase, slay - 1,
        //             erow - _rowBase, ecol - _columnBase, elay - 1, net);
        // _quad_tree_nets[_Net2Idx.at(str)].add_segment(
        //     srow - _rowBase, scol - _columnBase, slay - 1,
        //     erow - _rowBase, ecol - _columnBase, elay - 1);
        // ! substituted
        // segments[str].push_back(Segment(srow - _rowBase, scol - _columnBase,
        //                                 slay - 1, erow - _rowBase,
        //                                 ecol - _columnBase, elay - 1));
    }

    // safe::unordered_map<std::string, TreeNet> all_nets;
    // TODO: construct quad trees
    // for(size_t i = 0; i < _quad_tree_nets.size(); ++i){
    //     _quad_tree_nets[i].construct_tree();
    // }
}

Chip::~Chip() {

    // debug
    // std::fstream out("out.txt", std::ios::out);
    // for (Layer* ptr : _layers) {
    //    for (int i = 0; i < _rowRange; ++i) {
    //        for (int j = 0; j < _columnRange; ++j) {
    //          out << ptr->getGrid(getIdx(i, j)).getSupply() << std::endl;
    //    }
    // }
    // }
    // debug
}

int Chip::getIdx(int row, int column) const {
    assert(column >= 0 && column < _columnRange && row >= 0 && row < _rowRange);
    return column * _rowRange + row;
}

int Chip::getLeft(int row, int column) const {
    return (column == 0) ? -1 : getIdx(row, column - 1);
}

int Chip::getRight(int row, int column) const {
    return (column == _columnRange - 1) ? -1 : getIdx(row, column + 1);
}

int Chip::getDown(int row, int column) const {
    return (row == 0) ? -1 : getIdx(row - 1, column);
}

int Chip::getUp(int row, int column) const {
    return (row == _rowRange - 1) ? -1 : getIdx(row + 1, column);
}

void Chip::moveCell(Cell& cell) {
    assert(cell.movable(limited()));
    if (cell.moved()) {
        return;
    }
    _movedCells.push_back(cell.getId());
}

void Chip::decNumMoved() {
    assert(_movedCells.size() > 0);
}

size_t Chip::getNumLayers() const {
    return _layers.size();
}

size_t Chip::getNumColumns() const {
    return _columnRange;
}

size_t Chip::getNumRows() const {
    return _rowRange;
}

size_t Chip::getArea() const {
    return _area;
}

size_t Chip::getVolume() const {
    return _area*_layer;
}

size_t Chip::getNumNets() const {
    return _grid_nets.size();
}

size_t Chip::getNumCells() const {
    return _cells.size();
}

size_t Chip::getNumMasterCells() const {
    return _MasterCells.size();
}

Layer& Chip::getLayer(int layer) {
    return *_layers[layer];
}

Coordinate& Chip::getCoordinate(unsigned i) {
    return *_coordinates[i];
}

Cell& Chip::getCell(unsigned i) {
    return *_cells[i];
}

GridNet& Chip::getNet(unsigned i) {
    return _grid_nets[i];
}

Grid& Chip::getGrid(int layer, unsigned idx) {
    return _layers[layer]->getGrid(idx);
}

Grid& Chip::getGrid(int layer, int row, int column) {
    return getGrid(layer, getIdx(row, column));
}

MasterCellType& Chip::getMasterCell(unsigned idx) {
    return _MasterCells[idx];
}

bool Chip::limited() const {
    return _movedCells.size() == _maxMove;
}

void Chip::log() const {
    maxNetDegree();
}

// void Chip::output(std::fstream& output) {
//     int n = _movedCells.size();
//     output << "NumMovedCellInst " << n << '\n';
//     for (int i = 0; i < n; ++i) {
//         const Cell& cell = _cells[i];
//         output << "CellInst " << cell.getCellName() << " "
//                << cell.getRow() + _rowBase << " "
//                << cell.getColumn() + _columnBase << '\n';
//     }
//     outputRoute(output);
// }

void Chip::constructCoordinate() {
    _area = _columnRange * _rowRange;
    _coordinates.reserve(_area);
    for (int i = 0; i < _columnRange; ++i) {
        for (int j = 0; j < _rowRange; ++j) {
            std::shared_ptr<Coordinate> c(new Coordinate(j, i, _layer));
            _coordinates.push_back(c);
        }
    }
    for (int i = 0; i < _columnRange; ++i) {
        for (int j = 0; j < _rowRange; ++j) {
            int left = getLeft(j, i), right = getRight(j, i);
            std::shared_ptr<Coordinate> c1 = (left == -1) ? nullptr : _coordinates[left];
            std::shared_ptr<Coordinate> c2 = (right == -1) ? nullptr : _coordinates[right];
            _coordinates[getIdx(j, i)]->addAdjH(c1, c2);
        }
    }
}

void Chip::connectCoordinateGrid() {
    for (int i = 0; i < _area; ++i) {
        std::shared_ptr<Coordinate> c = _coordinates[i];
        for (int j = 0; j < _layer; ++j) {
            Layer& l = *_layers[j];
            Grid& g = l.getGrid(i);
            g.assignCoordinate(c);
            c->addGrid(std::shared_ptr<Grid>(&g));
        }
    }
}

void Chip::assignRoute(int srow,
                           int scol,
                           int slay,
                           int erow,
                           int ecol,
                           int elay,
                           GridNet& net) {
    // net.addSegment(srow, scol, slay, erow, ecol, elay);
    if (srow > erow) {
        std::swap(srow, erow);
    }
    if (scol > ecol) {
        std::swap(scol, ecol);
    }
    if (slay > elay) {
        std::swap(slay, elay);
    }
    for (int i = slay; i <= elay; ++i) {
        Layer& l = *_layers[i];
        for (int j = scol; j <= ecol; ++j) {
            for (int k = srow; k <= erow; ++k) {
                Grid& g = l.getGrid(getIdx(k, j));
                g.addNet(net);
            }
        }
    }
}

// void Chip::outputRoute(std::fstream& output) {
//     output << "NumRoutes ";
//     int numRoutes = 0;
//     for (int i = 0, n = _grid_nets.size(); i < n; ++i) {
//         numRoutes += _grid_nets[i].getNumSegments();
//     }
//     assert((numRoutes % 6) == 0);
//     numRoutes /= 6;
//     output << numRoutes << '\n';
//     for (int i = 0, n = _grid_nets.size(); i < n; ++i) {
//         GridNet& net = _grid_nets[i];
//         safe::vector<unsigned>& segments = net.getSegments();
//         assert((segments.size() % 6) == 0);
//         for (int j = 0, m = segments.size() / 6; j < m; ++j)
//             output << segments[6 * j] + _rowBase << " "
//                    << segments[6 * j + 1] + _columnBase << " "
//                    << segments[6 * j + 2] + 1 << " "
//                    << segments[6 * j + 3] + _rowBase << " "
//                    << segments[6 * j + 4] + _columnBase << " "
//                    << segments[6 * j + 5] + 1 << " " << net.getName() <<
//                    '\n';
//     }
// }

void Chip::maxNetDegree() const {
    int maxDegree = 0;
    for (int i = 0, n = _grid_nets.size(); i < n; ++i) {
        int d = _grid_nets[i].getNumPin();
        if (d > maxDegree) {
            maxDegree = d;
        }
    }
    // std::cout << "Max Net Degree : " << maxDegree << '\n';
}
