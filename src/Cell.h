/***********************************************************************

  FileName    [Cell.h]

  Author      [Yang, Chien Yi]

  This file defines the cells and their elements, pins and nets.

***********************************************************************/

#ifndef CELL_H
#define CELL_H

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "MasterCell.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Net;
class Cell;

class Pin
{
public:
    //Constructors
    Pin(PinType& PT, Cell& cell): _PT(PT), _cell(cell) {}
    Pin(Pin& a): _PT(a._PT), _cell(a._cell) {}

    //modifier
    void setNet(Net& net) { _net = net; }

    //accesser
    PinType& getPinType() const                         { return _PT; }
    Net& get_net() const                                { return _net; }
    Cell& get_cell() const                              { return _cell; }
    unsigned getX() const                               { return _cell.getX(); }
    unsigned getY() const                               { return _cell.getY(); }
    int getLayer() const                                { return _PT.getLayer(); }    

private:
    PinType&                            _PT;
    Cell&                               _cell;
    Net&                                _net;
};

class Net
{
public:
    // Constructors(no copy constructor)
    Net(const std::string& NetName, unsigned id, unsigned PinNum, unsigned layer): _NetName(NetName), _Id(id), _layer(layer)
    {
        _pins.reserve(PinNum);
    }

    //modifier
    void addPin(Pin* pin)                               { _pins.push_back(pin); }
    void addSegment()                                   {}

    //accesser
    std::string& getName() const                        { return _NetName; }
    unsigned getId() const                              { return _Id; }
    unsigned getMinlayer() const                        { return _layer; }

private:
    const std::string                   _NetName;
    const unsigned                      _Id;
    const unsigned                      _layer;
    std::vector<Pin*>                   _pins;
}

class Cell
{
public:
    //Constructors(no copy constructor)
    Cell(const std::string& CellName, MasterCellType& MCT, bool movable, unsigned id):
    _CellName(CellName), _MCT(MCT), _movable(movable), _Id(id)
    {
        int n = _MCT.getNumPins();
        _pins.reserve(n);
        for(int i = 0; i < n; ++i) _pins.push_back(Pin(_MCT.getPinType(i),*this));
    }

    //modifier
    void setX(unsigned x)                               { _x = x; }
    void setY(unsigned y)                               { _y = y; }

    //accesser
    std::string getCellName() const                     { return _CellName; }
    bool movable() const                                { return _movable; }
    unsigned getX() const                               { return _x; }
    unsigned getY() const                               { return _y; }
    Pin& getPin(size_t i) const                         { assert(i < _pins.size()); return _pins[i]; }
    Pin& getPin(std::string str) const                  { return _pins[_MCT.getPin(str)]; }
    int getBlkgDemand() const                           { return _MCT.getDemand(); }
    int getSameGridDemand(Cell& a) const                { return _MCT.getSameDemand(a._MCT); }
    int getadjHGridDemand(Cell& a) const                { return _MCT.getDemand(a._MCT); }
    size_t getNumPins() const                           { return _pins.size(); }
    int getLayerDemand(int i) const                     { return _MCT.getLayerDemand(i); }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

private:
    const std::string                   _CellName;
    MasterCellType&                     _MCT;
    const unsigned                      _Id;
    const bool                          _movable;
    unsigned                            _x;
    unsigned                            _y;
    std::vector<Pin>                    _pins;
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);

#endif