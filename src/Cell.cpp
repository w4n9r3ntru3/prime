/***********************************************************************

  FileName    [Cell.cpp]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file describes the cells and their elements, pins and nets.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Cell.h"

#include <assert.h>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

// Pin
Pin::Pin(PinType& PT, Cell& cell) : _PT(&PT), _cell(&cell) {}

Pin::Pin(Pin&& p) : _PT(p._PT), _cell(p._cell), _net(p._net) {
    p._PT = nullptr;
    p._cell = nullptr;
    p._net = nullptr;
}

Pin& Pin::operator=(Pin&& p) {
    _PT = p._PT;
    _cell = p._cell;
    _net = p._net;

    p._PT = nullptr;
    p._cell = nullptr;
    p._net = nullptr;

    return *this;
}

void Pin::setNet(GridNet* net) {
    _net = net;
}

PinType& Pin::getPinType() const {
    assert(_PT != nullptr);
    return *_PT;
}

GridNet& Pin::get_net() const {
    assert(_net != nullptr);
    return *_net;
}

Cell& Pin::get_cell() const {
    assert(_cell != nullptr);
    return *_cell;
}

unsigned Pin::getRow() const {
    return get_cell().getRow();
}

unsigned Pin::getColumn() const {
    return get_cell().getColumn();
}

int Pin::getLayer() const {
    return getPinType().getLayer();
}

// Net
GridNet::GridNet(const std::string NetName,
                 unsigned id,
                 unsigned PinNum,
                 unsigned layer) noexcept
    : _NetName(NetName), _Id(id), _minLayer(layer) {
    _pins.reserve(PinNum);
}

GridNet::GridNet(const GridNet& g) noexcept
    : _NetName(g._NetName),
      _Id(g._Id),
      _minLayer(g._minLayer),
      _pins(g._pins)
//   ,
//   _segments(g._segments)
{}

void GridNet::addPin(Pin* pin) {
    _pins.push_back(pin);
}

// void GridNet::addSegment(int srow,
//                          int scol,
//                          int slay,
//                          int erow,
//                          int ecol,
//                          int elay) {
//     _segments.push_back(srow);
//     _segments.push_back(scol);
//     _segments.push_back(slay);
//     _segments.push_back(erow);
//     _segments.push_back(ecol);
//     _segments.push_back(elay);
// }

const std::string& GridNet::getName() const {
    return _NetName;
}

unsigned GridNet::getId() const {
    return _Id;
}

unsigned GridNet::getMinlayer() const {
    return _minLayer;
}

size_t GridNet::getNumPin() const {
    return _pins.size();
}

Pin& GridNet::getPin(unsigned i) {
    return *_pins[i];
}

// size_t GridNet::getNumSegments() const {
//     return _segments.size();
// }

// safe::vector<unsigned>& GridNet::getSegments() {
//     return _segments;
// }

// Cell
Cell::Cell(const std::string CellName,
           MasterCellType& MCT,
           bool movable,
           unsigned id)
    : _CellName(CellName),
      _MCT(&MCT),
      _Id(id),
      _movable(movable),
      _moved(false) {
    assert(_MCT != nullptr);

    size_t p = getMasterCell().getNumPins();
    _pins.reserve(p);
    size_t l = getMasterCell().getNumLayers();
    _Layer2pin.reserve(l);
    for (size_t i = 0; i < l; ++i) {
        safe::vector<Pin*> v = safe::vector<Pin*>();
        _Layer2pin.push_back(std::move(v));
    }
    for (size_t i = 0; i < p; ++i) {
        _pins.push_back(std::move(Pin(getMasterCell().getPinType(i), *this)));
        _Layer2pin[_pins[i].getLayer()].push_back(&_pins[i]);
    }
}

Cell::Cell(Cell&& c)
    : _CellName(std::move(c._CellName)),
      _MCT(c._MCT),
      _Id(c._Id),
      _movable(c._movable),
      _moved(c._moved),
      _row(c._row),
      _column(c._column),
      _pins(std::move(c._pins)),
      _Layer2pin(std::move(c._Layer2pin)) {}

Cell& Cell::operator=(Cell&& c) {
    _CellName = std::move(c._CellName);
    _MCT = c._MCT;
    _Id = c._Id;

    _movable = c._movable;
    _moved = c._moved;
    _row = c._row;
    _column = c._column;
    _pins = std::move(c._pins);
    _Layer2pin = std::move(c._Layer2pin);

    return *this;
}

void Cell::setRow(unsigned x) {
    _row = x;
}

void Cell::setColumn(unsigned y) {
    _column = y;
}

void Cell::setCoordinate(unsigned x, unsigned y) {
    _row = x;
    _column = y;
}

void Cell::move() {
    _moved = true;
}

const std::string& Cell::getCellName() const {
    return _CellName;
}

unsigned Cell::getId() const {
    return _Id;
}

const MasterCellType& Cell::getMasterCell() const {
    assert(_MCT != nullptr);
    return *_MCT;
}

MasterCellType& Cell::getMasterCell() {
    assert(_MCT != nullptr);
    return *_MCT;
}

int Cell::getMasterCellId() const {
    return getMasterCell().getId();
}

bool Cell::moved() const {
    return _moved;
}

bool Cell::movable(bool constraint) const {
    if (constraint) {
        return _moved;
    } else {
        return _movable;
    }
}

unsigned Cell::getRow() const {
    return _row;
}

unsigned Cell::getColumn() const {
    return _column;
}

Pin& Cell::getPin(size_t i) {
    return _pins[i];
}

Pin& Cell::getPin(std::string& str) {
    return _pins[getMasterCell().getPin(str)];
}

int Cell::getLayerDemand(int i) const {
    return getMasterCell().getLayerDemand(i);
}

safe::vector<unsigned>& Cell::getSameGridMC(unsigned layer) {
    return getMasterCell().getSameGridMC(layer);
}

safe::vector<unsigned>& Cell::getadjHGridMC(unsigned layer) {
    return getMasterCell().getadjHGridMC(layer);
}

safe::vector<int>& Cell::getSameGridDemand(unsigned layer) {
    return getMasterCell().getSameGridDemand(layer);
}

safe::vector<int>& Cell::getadjHGridDemand(unsigned layer) {
    return getMasterCell().getadjHGridDemand(layer);
}

size_t Cell::getNumPins() const {
    return _pins.size();
}

const safe::vector<Pin*>& Cell::getPinLayer(int i) const {
    return _Layer2pin[i];
}

safe::vector<Pin*>& Cell::getPinLayer(int i) {
    return _Layer2pin[i];
}

std::ostream& operator<<(std::ostream& os, const Cell& cell) {
    os << "CellName : " << cell.getCellName()
       << " MasterCellType : " << cell.getMasterCell().getMCName() << '\n';
    return os;
}
