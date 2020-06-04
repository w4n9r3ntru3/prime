/***********************************************************************

  FileName    [Cell.cpp]

  Author      [Yang Chien Yi]

  This file describes the cells and their elements, pins and nets.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Cell.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

//Pin
Pin::Pin(PinType& PT, Cell* cell) : _PT(PT), _cell(cell) {

}

Pin::Pin(Pin& a) : _PT(a._PT), _cell(a._cell) {

}

void Pin::setNet(Net* net) {
    _net = net;
}

PinType& Pin::getPinType() const {
    return _PT;
}

Net& Pin::get_net() const {
    return *_net; 

}

Cell& Pin::get_cell() const {
    return *_cell;
}

unsigned Pin::getRow() const {
    return _cell->getRow();
}

unsigned Pin::getColumn() const {
    return _cell->getColumn();
}

int Pin::getLayer() const {
    return _PT.getLayer();
}

//Net
Net::Net(const std::string NetName, unsigned id, unsigned PinNum, unsigned layer)
    : _NetName(NetName), _Id(id), _layer(layer) {
    _pins.reserve(PinNum);
}

void Net::addPin(Pin* pin) {
    _pins.push_back(pin);
}

void Net::addSegment() {
    // TODO
}

const std::string& Net::getName() const {
    return _NetName;
}

unsigned Net::getId() const {
    return _Id;
}

unsigned Net::getMinlayer() const {
    return _layer;
}

size_t Net::getNumPin() const {
    return _pins.size();
}

Pin& Net::getPin(unsigned i) {
    return *_pins[i];
}

//Cell
Cell::Cell(const std::string CellName,
        MasterCellType& MCT,
        bool movable,
        unsigned id)
    : _CellName(CellName),
        _MCT(MCT),
        _movable(movable),
        _moved(false),
        _Id(id) {
    size_t p = _MCT.getNumPins();
    _pins.reserve(p);
    size_t l = _MCT.getNumLayers();
    _Layer2pin.reserve(l);
    for (size_t i = 0; i < l; ++i) {
        safe::vector<Pin*>* v = new safe::vector<Pin*>();
        _Layer2pin.push_back(v);
    }
    for (size_t i = 0; i < p; ++i) {
        _pins.push_back(Pin(_MCT.getPinType(i), this));
        _Layer2pin[_pins[i].getLayer()]->push_back(&_pins[i]);
    }
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

MasterCellType& Cell::getMasterCell() {
    return _MCT;
}

int Cell::getMasterCellId() const {
    return _MCT.getId();
}

bool Cell::movable(bool constraint) const {
    if (constraint) {
        return constraint && _moved;
    }
    return _movable;
}

unsigned Cell::getRow() const {
    return _row;
}

unsigned Cell::getColumn() const {
    return _column;
}

Pin& Cell::getPin(size_t i) {
    assert(i < _pins.size());
    return _pins[i];
}

Pin& Cell::getPin(std::string& str) {
    return _pins[_MCT.getPin(str)];
}

int Cell::getLayerDemand(int i) const {
    return _MCT.getLayerDemand(i);
}

safe::vector<unsigned>& Cell::getSameGridMC(unsigned layer) {
    return _MCT.getSameGridMC(layer);
}

safe::vector<unsigned>& Cell::getadjHGridMC(unsigned layer) {
    return _MCT.getadjHGridMC(layer);
}

safe::vector<int>& Cell::getSameGridDemand(unsigned layer) {
    return _MCT.getSameGridDemand(layer);
}

safe::vector<int>& Cell::getadjHGridDemand(unsigned layer) {
    return _MCT.getadjHGridDemand(layer);
}

size_t Cell::getNumPins() const {
    return _pins.size();
}

safe::vector<Pin*>& Cell::getPinLayer(int i) const {
    return *_Layer2pin[i];
}

std::ostream& operator<<(std::ostream& os, const Cell& cell) {
    os << "CellName : " << cell.getCellName()
       << " MasterCellType : " << cell._MCT.getMCName() << '\n';
}
