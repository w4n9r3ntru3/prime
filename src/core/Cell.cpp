/***********************************************************************

  FileName    [Cell.cpp]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file describes the cells and their elements, pins and nets.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "../include/Cell.h"

#include <assert.h>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

// Pin
Pin::Pin(unsigned idx, unsigned layer, Cell& cell) : _idx(idx), _layer(layer) ,_cell(cell.getIdx()) {}

// Pin::Pin(const Pin& pin) : _idx(pin._idx), _layer(pin._layer), _cell(pin._cell), _net(pin._net) {}

Pin::Pin(Pin&& pin) : _idx(pin._idx), _layer(pin._layer), _cell(pin._cell), _net(pin._net) {}

Pin& Pin::operator=(Pin&& pin){
    _idx = pin._idx;
    _layer = pin._layer;
    _cell = pin._cell;
    _net = pin._net;
    return *this;
}

void Pin::setNet(GridNet& net) {
    _net = net.getIdx();
}

unsigned Pin::getIdx() const {
    return _idx;
}

unsigned Pin::get_net_idx() const{
    return _net;
}

unsigned Pin::get_cell_idx() const{
    return _cell;
}

unsigned Pin::getLayer() const {
    return _layer;
}

// Net
GridNet::GridNet(unsigned idx,
                 unsigned PinNum,
                 unsigned minLayer)
    : _idx(idx), _minLayer(minLayer) {
    _pins.reserve(PinNum);
}

// GridNet::GridNet(const GridNet& g)
//     : _idx(g._idx),
//       _minLayer(g._minLayer),
//       _pins(g._pins) {}

// GridNet::GridNet(const GridNet& g)
//     : _idx(std::move(g._idx)),
//       _minLayer(std::move(g._minLayer)),
//       _pins(std::move(g._pins)) {}

GridNet& GridNet::operator=(GridNet&& net){
    _idx = net._idx;
    _minLayer = net._minLayer;
    _pins = std::move(net._pins);
    return *this;
}

void GridNet::addPin(Pin& pin) {
    _pins.push_back(std::make_pair(pin.get_cell_idx(),pin.getIdx()));
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

unsigned GridNet::getIdx() const {
    return _idx;
}

unsigned GridNet::getMinlayer() const {
    return _minLayer;
}

size_t GridNet::getNumPin() const {
    return _pins.size();
}

Pin& GridNet::getPin(unsigned i, safe::vector<Cell>& cells) {

    return (cells[_pins[i].first].getPin(_pins[i].second));
}

// size_t GridNet::getNumSegments() const {
//     return _segments.size();
// }

// safe::vector<unsigned>& GridNet::getSegments() {
//     return _segments;
// }

// Cell
Cell::Cell(MasterCellType& MCT,
         unsigned idx,
         bool movable,
         unsigned layers)
    : _MCT(MCT),
      _idx(idx),
      _movable(movable),
      _moved(false) {
    size_t p = getMasterCell().getNumPins();
    _pins.reserve(p);
    _Layer2pin.reserve(layers);
    for (size_t i = 0; i < layers; ++i) {
        safe::vector<unsigned> v;
        _Layer2pin.push_back(std::move(v));
    }
    for (size_t i = 0; i < p; ++i) {
        _pins.push_back(std::move(Pin(i,getMasterCell().getPinLayer(i), *this)));
        assert(_pins[i].getIdx() == i);
        _Layer2pin[_pins[i].getLayer()].push_back(i);
    }
}

// Cell::Cell(const Cell& c)
//     : _MCT(c._MCT),
//       _idx(c._idx),
//       _movable(c._movable),
//       _moved(c._moved),
//       _row(c._row),
//       _column(c._column),
//       _pins(c._pins),
//       _Layer2pin(c._Layer2pin) {}

Cell::Cell(Cell&& c)
    : _MCT(c._MCT),
      _idx(c._idx),
      _movable(c._movable),
      _moved(c._moved),
      _row(c._row),
      _column(c._column),
      _pins(std::move(c._pins)),
      _Layer2pin(std::move(c._Layer2pin)) {}

// Cell& Cell::operator=(Cell&& c){
//     _MCT = std::move(c._MCT);
//     _idx = c._idx;
//     _movable = c._movable;
//     _moved = c._moved;
//     _row = c._row;
//     _column = c._column;
//     _pins = std::move(c._pins);
//     _Layer2pin = std::move(c._Layer2pin);
// }

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

unsigned Cell::getIdx() const {
    return _idx;
}

const MasterCellType& Cell::getMasterCell() const {
    return _MCT;
}

MasterCellType& Cell::getMasterCell() {
    return _MCT;
}

unsigned Cell::getMasterCellId() const {
    return getMasterCell().getIdx();
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

Pin& Cell::getPin(unsigned i) {
    return _pins[i];
}

int Cell::getLayerDemand(unsigned i) const {
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

const safe::vector<unsigned>& Cell::getPinLayer(unsigned i) const {
    return _Layer2pin[i];
}
