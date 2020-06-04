/***********************************************************************

  FileName    [Cell.h]

  Author      [Yang Chien Yi]

  This file defines the cells and their elements, pins and nets.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "MasterCell.h"
#include "safe.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Net;
class Cell;

class Pin {
   public:
    // Constructors
    Pin(PinType& PT, Cell* cell) : _PT(PT), _cell(cell) {}
    Pin(Pin& a) : _PT(a._PT), _cell(a._cell) {}

    // modifier
    void setNet(Net* net) { _net = net; }

    // accesser
    PinType& getPinType() const { return _PT; }
    Net& get_net() const { return *_net; }
    Cell& get_cell() const { return *_cell; }
    unsigned getRow() const { return _cell->getRow(); }
    unsigned getColumn() const { return _cell->getColumn(); }
    int getLayer() const { return _PT.getLayer(); }

   private:
    PinType& _PT;
    Cell* _cell;
    Net* _net;
};

class Net {
   public:
    // Constructors(no copy constructor)
    Net(const std::string NetName, unsigned id, unsigned PinNum, unsigned layer)
        : _NetName(NetName), _Id(id), _layer(layer) {
        _pins.reserve(PinNum);
    }

    // modifier
    void addPin(Pin* pin) { _pins.push_back(pin); }
    void addSegment() {
        // TODO
    }

    // accesser
    const std::string& getName() const { return _NetName; }
    unsigned getId() const { return _Id; }
    unsigned getMinlayer() const { return _layer; }
    size_t getNumPin() const { return _pins.size(); }
    Pin& getPin(unsigned i) { return *_pins[i]; }

   private:
    const std::string _NetName;
    const unsigned _Id;
    const unsigned _layer;
    safe::vector<Pin*> _pins;
};

class Cell {
   public:
    // Constructors(no copy constructor)
    Cell(const std::string CellName,
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

    // modifier
    void setRow(unsigned x) { _row = x; }
    void setColumn(unsigned y) { _column = y; }
    void setCoordinate(unsigned x, unsigned y) {
        _row = x;
        _column = y;
    }
    void move() { _moved = true; }

    // accesser
    const std::string& getCellName() const { return _CellName; }
    MasterCellType& getMasterCell() { return _MCT; }
    int getMasterCellId() const { return _MCT.getId(); }
    bool movable(bool constraint) const {
        if (constraint) {
            return constraint && _moved;
        }
        return _movable;
    }
    unsigned getRow() const { return _row; }
    unsigned getColumn() const { return _column; }
    Pin& getPin(size_t i) {
        assert(i < _pins.size());
        return _pins[i];
    }
    Pin& getPin(std::string& str) { return _pins[_MCT.getPin(str)]; }
    int getLayerDemand(int i) const { return _MCT.getLayerDemand(i); }
    safe::vector<unsigned>& getSameGridMC(unsigned layer) {
        return _MCT.getSameGridMC(layer);
    }
    safe::vector<unsigned>& getadjHGridMC(unsigned layer) {
        return _MCT.getadjHGridMC(layer);
    }
    safe::vector<int>& getSameGridDemand(unsigned layer) {
        return _MCT.getSameGridDemand(layer);
    }
    safe::vector<int>& getadjHGridDemand(unsigned layer) {
        return _MCT.getadjHGridDemand(layer);
    }
    size_t getNumPins() const { return _pins.size(); }
    int getLayerDemand(int i) const { return _MCT.getLayerDemand(i); }
    safe::vector<Pin*>& getPinLayer(int i) const { return *_Layer2pin[i]; }

    // friend
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

   private:
    const std::string _CellName;
    MasterCellType& _MCT;
    const unsigned _Id;
    const bool _movable;
    bool _moved;
    unsigned _row;
    unsigned _column;
    safe::vector<Pin> _pins;
    safe::vector<safe::vector<Pin*>*> _Layer2pin;
};

std::ostream& operator<<(std::ostream& os, const Cell& cell) {
    os << "CellName : " << cell.getCellName()
       << " MasterCellType : " << cell._MCT.getMCName() << '\n';
}
