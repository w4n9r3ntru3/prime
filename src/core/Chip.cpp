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

Chip::Chip(mstream& input) {
    readFile(input);
}

void Chip::readFile(mstream& input) {
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
    constructCoordinate();
    for (unsigned i = 0; i < _layer; ++i) {
        input >> str;  // Lay
        assert(str == "Lay");
        input >> str;  //<LayerName>
        int idx;
        input >> idx;  //<Idx>
        bool direction;
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
        _layers.push_back(Layer(idx - 1, direction, supply, _area));
    }

    /*NumNonDefaultSupplyGGrid <nonDefaultSupplyGGridCount>
      <rowIdx> <colIdx> <LayIdx> <incrOrDecrValue>*/
    input >> str;  // NumNonDefaultSupplyGGrid
    assert(str == "NumNonDefaultSupplyGGrid");
    int count, row, column, layer, val;
    input >> count;
    for (int i = 0; i < count; ++i) {
        input >> row >> column >> layer >> val;
        _layers[layer - 1]
            .getGrid(getIdx(row - _rowBase, column - _columnBase))
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
        MasterCellType mct = MasterCellType(i, _layer);
        int pin, blockage;
        input >> pin >> blockage;  // <pinCount> <blockageCount>
        mct.reservePin(pin);
        for (int j = 0; j < pin; ++j) {
            input >> str;  // Pin
            assert(str == "Pin");
            input >> str >> buf;  // <pinName> <pinLayer>
            mct.AddPin(str2Idx("M", buf));
        }
        for (int j = 0; j < blockage; ++j) {
            input >> str;  // Blkg
            assert(str == "Blkg");
            input >> str >> buf >>
                demand;  // <blockageName> <blockageLayer> <demand>
            mct.AddBlkg(str2Idx("M", buf), demand);
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
        mc1 = str2Idx("MC", buf);
        input >> buf;  // <masterCellName2>
        // FIXME the original version does not check, intentional?
        mc2 = str2Idx("MC", buf);
        input >> buf >> demand;  // <layerName> <demand>
        layer = str2Idx("M", buf);
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
    unsigned pinIdx = 0;
    for (int i = 0; i < count; ++i) {
        input >> str;  // CellInst
        assert(str == "CellInst");
        input >> str;  // <instName>
        // assert(_Cell2Idx.count(str) == 0);
        input >> buf;  // <masterCellName>
        // FIXME the original version does not check, intentional?
        MasterCellType& MCT = _MasterCells[str2Idx("MC", buf)];
        input >> row >> column >> buf;
        // <gGridRowIdx> <gGridColIdx> <movableCstr>
        bool movable;
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
        _cells.push_back(Cell(MCT, i, movable, _layer, pinIdx, _pins));
        unsigned rIdx = row - _rowBase, cIdx = column - _columnBase;
        Cell& cell = _cells[i];
        assert(rIdx < _rowRange);
        assert(cIdx < _columnRange);
        cell.setCoordinate(rIdx, cIdx);
        _coordinates[getIdx(rIdx, cIdx)].initCell(cell, _coordinates, _layers);
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
        int minLay;
        input >> buf;  // <minRoutingLayConstraint>
        if (buf == "NoCstr") {
            minLay = 0;
        } else {
            minLay = str2Idx("M", buf);
        }

        _grid_nets.push_back(GridNet(i, numPins, minLay));
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
            Pin& pin = _pins[_cells[str2Idx("C", inst)].getPinIdx(
                str2Idx("P", masterPin))];
            // Cell& cell_ = pin.get_cell();
            // std::cout << &cell << " " << &cell_ << std::endl;
            _grid_nets[i].addPin(pin);
            pin.setNet(_grid_nets[i]);
            Grid& g = _layers[pin.getLayer()].getGrid(
                getIdx(getPinRow(pin), getPinColumn(pin)));
            g.addPin(i);
        }
        // return;
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

        GridNet& net = _grid_nets[str2Idx("N", str)];
        assignRoute(srow - _rowBase, scol - _columnBase, slay - 1,
                    erow - _rowBase, ecol - _columnBase, elay - 1, net);
    }

    // safe::unordered_map<std::string, TreeNet> all_nets;
}

Chip::~Chip() {
    // debug
    std::fstream out("out.txt", std::ios::out);
    for (Layer& ptr : _layers) {
        for (unsigned i = 0; i < _rowRange; ++i) {
            for (unsigned j = 0; j < _columnRange; ++j) {
                out << ptr.getGrid(getIdx(i, j)).getSupply() << std::endl;
            }
        }
    }
    // debug
}

int Chip::getIdx(unsigned row, unsigned column) const {
    assert(column < _columnRange && row < _rowRange);
    return column * _rowRange + row;
}

int Chip::getLeft(unsigned row, unsigned column) const {
    return (column == 0) ? -1 : getIdx(row, column - 1);
}

int Chip::getRight(unsigned row, unsigned column) const {
    return (column == _columnRange - 1) ? -1 : getIdx(row, column + 1);
}

int Chip::getDown(unsigned row, unsigned column) const {
    return (row == 0) ? -1 : getIdx(row - 1, column);
}

int Chip::getUp(unsigned row, unsigned column) const {
    return (row == _rowRange - 1) ? -1 : getIdx(row + 1, column);
}

bool Chip::moveCellLegal(Cell& cell,
                         unsigned sRow,
                         unsigned sCol,
                         unsigned eRow,
                         unsigned eCol) {
    assert(cell.movable());
    if(sRow == eRow && sCol == eCol) {
        return true;
    }
    if (!cell.movable(limited())) {
        return false;
    }
    Coordinate& c_target = _coordinates[getIdx(eRow, eCol)];
    if (!c_target.CanAddCell(cell, _coordinates, _layers)) {
        return false;
    }
    if (!cell.moved()) {
        assert(_movedCells.size() < _maxMove);
        _movedCells.push_back(cell.getIdx());
    } else {
        cell.move();
    }
    cell.setCoordinate(eRow, eCol);
    Coordinate& c_origin = _coordinates[getIdx(sRow, sCol)];
    c_origin.rmCell(cell, _coordinates, _layers, _pins);
    c_target.addCell(cell, _coordinates, _layers, _pins);
    return true;
}

void Chip::moveCell(Cell& cell,
                    unsigned sRow,
                    unsigned sCol,
                    unsigned eRow,
                    unsigned eCol) {
    assert(cell.movable());
    if(sRow == eRow && sCol == eCol) {
        return;
    }
    Coordinate& c_target = _coordinates[getIdx(eRow, eCol)];
    if (!cell.moved()) {
        assert(_movedCells.size() < _maxMove);
        _movedCells.push_back(cell.getIdx());
    } else {
        cell.move();
    }
    cell.setCoordinate(eRow, eCol);
    Coordinate& c_origin = _coordinates[getIdx(sRow, sCol)];
    c_origin.rmCell(cell, _coordinates, _layers, _pins);
    c_target.addCell(cell, _coordinates, _layers, _pins);
    return;
}

bool Chip::moveCelltry(Cell& cell,
                       unsigned sRow,
                       unsigned sCol,
                       unsigned eRow,
                       unsigned eCol) {
    assert(cell.movable());
    assert(cell.movable(limited()));
    if(sRow == eRow && sCol == eCol) {
        return true;
    }
    Coordinate& c_target = _coordinates[getIdx(eRow, eCol)];
    if (!c_target.CanAddCell(cell, _coordinates, _layers)) {
        return false;
    }
    Coordinate& c_origin = _coordinates[getIdx(sRow, sCol)];
    c_origin.rmCell(cell, _coordinates, _layers, _pins);
    c_target.addCell(cell, _coordinates, _layers, _pins);
    return true;
}

/*void Chip::revert(Cell& cell,
                  unsigned sRow,
                  unsigned sCol,
                  unsigned eRow,
                  unsigned eCol) {
    // TODO
}*/

void Chip::decNumMoved() {
    assert(_movedCells.size() > 0);
}

size_t Chip::getRowBase() const {
    return _rowBase;
}
size_t Chip::getColumnBase() const {
    return _columnBase;
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
    return _area * _layer;
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

size_t Chip::getNumPins() const {
    size_t numPins = 0;
    for (size_t i = 0; i < _cells.size(); ++i) {
        numPins += _cells[i].getNumPins();
    }
    return numPins;
}

Layer& Chip::getLayer(int layer) {
    return _layers[layer];
}

Coordinate& Chip::getCoordinate(unsigned i) {
    return _coordinates[i];
}

Cell& Chip::getCell(unsigned i) {
    return _cells[i];
}

const Pin& Chip::getPin(const GridNet& net, unsigned idx) const {
    return _pins[net.getPinIdx(idx)];
}

const Pin& Chip::getPin(unsigned idx) const {
    return _pins[idx];
}

unsigned Chip::getPinRow(const Pin& pin) const {
    return _cells[pin.get_cell_idx()].getRow();
}

unsigned Chip::getPinColumn(const Pin& pin) const {
    return _cells[pin.get_cell_idx()].getColumn();
}

GridNet& Chip::getNet(unsigned i) {
    return _grid_nets[i];
}

Grid& Chip::getGrid(unsigned layer, unsigned idx) {
    return _layers[layer].getGrid(idx);
}

Grid& Chip::getGrid(unsigned layer, unsigned row, unsigned column) {
    return getGrid(layer, getIdx(row, column));
}

MasterCellType& Chip::getMasterCell(unsigned idx) {
    return _MasterCells[idx];
}

unsigned Chip::getMaxMove() const {
    return _maxMove;
}

unsigned Chip::getNumMoved() const {
    return _movedCells.size();
}

bool Chip::limited() const {
    assert(_movedCells.size() <= _maxMove);
    return _movedCells.size() == _maxMove;
}

void Chip::log() const {
    maxNetDegree();
}

bool Chip::isValidPosition(int layer, int row, int column) {
    bool a = (layer >= 0 && (unsigned)layer < _layer);
    bool b =
        (row >= 0 && (unsigned)row >= _rowBase && (unsigned)row < _rowRange);
    bool c = (column >= 0 && (unsigned)column >= _columnBase &&
              (unsigned)column < _columnRange);
    return a && b && c;
}

void Chip::output(std::fstream& output) {
    int n = _movedCells.size();
    output << "NumMovedCellInst " << n << '\n';
    for (int i = 0; i < n; ++i) {
        const Cell& cell = _cells[i];
        output << "CellInst " << cell.getCellName() << " "
               << cell.getRow() + _rowBase << " "
               << cell.getColumn() + _columnBase << '\n';
    }
    size_t total_route = 0;
    for (size_t i = 0; i < _grid_nets.size(); ++i) {
        total_route += _grid_nets[i].getNumSegments() / 6;
    }
    output << "NumRoutes " << total_route << '\n';
    for (size_t i = 0; i < _grid_nets.size(); ++i) {
        safe::vector<unsigned>& segments = _grid_nets[i].getSegments();
        for (size_t j = 0; j < segments.size(); j += 6) {
            output << segments[j] + _rowBase << " "
                   << segments[j + 1] + _columnBase << " "
                   << segments[j + 2] + 1 << " " << segments[j + 3] + _rowBase
                   << " " << segments[j + 4] + _columnBase << " "
                   << segments[j + 5] + 1 << " "
                   << "N" << _grid_nets[i].getIdx() + 1 << '\n';
        }
    }
}

void Chip::constructCoordinate() {
    _area = _columnRange * _rowRange;
    _coordinates.reserve(_area);
    for (unsigned i = 0; i < _columnRange; ++i) {
        for (unsigned j = 0; j < _rowRange; ++j) {
            _coordinates.push_back(
                Coordinate(j, i, getIdx(j, i), getLeft(j, i), getRight(j, i)));
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
    if (srow > erow) {
        std::swap(srow, erow);
    }
    if (scol > ecol) {
        std::swap(scol, ecol);
    }
    if (slay > elay) {
        std::swap(slay, elay);
    }
    net.addSegment(srow, scol, slay, erow, ecol, elay);
    for (int i = slay; i <= elay; ++i) {
        Layer& l = _layers[i];
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

unsigned str2Idx(std::string title, std::string& str) {
    size_t pos = str.find(title) + title.size();
    std::string inst = str.substr(pos, str.size() - pos);
    return std::stoul(inst) - 1;
}

unsigned Chip::HPWL() {
    unsigned ret = 0;
    for (unsigned i = 0; i < getNumNets(); ++i) {
        ret += HPWL_NET(i);
    }
    return ret;
}

inline unsigned Chip::HPWL_NET(unsigned idx) {
    const GridNet& net = getNet(idx);
    unsigned maxRow = 0, minRow = getNumRows(), maxColumn = 0,
             minColumn = getNumColumns();
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        Cell& cell = getCell(getPin(net.getPinIdx(i)).get_cell_idx());
        unsigned row = cell.getRow(), column = cell.getColumn();
        assert(row < getNumRows());
        assert(column < getNumColumns());
        if (row > maxRow) {
            maxRow = row;
        }
        if (row < minRow) {
            minRow = row;
        }
        if (column > maxColumn) {
            maxColumn = column;
        }
        if (column < minColumn) {
            minColumn = column;
        }
    }
    assert(maxRow >= minRow && maxColumn >= minColumn);
    return maxRow - minRow + maxColumn - minColumn;
}