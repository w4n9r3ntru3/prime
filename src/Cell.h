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
    Pin(PinType& PT, Cell* cell);
    Pin(const Pin& a);

    // modifier
    void setNet(Net* net);

    // accesser
    PinType& getPinType() const;
    Net& get_net() const;
    Cell& get_cell() const;
    unsigned getRow() const;
    unsigned getColumn() const;
    int getLayer() const;

   private:
    PinType& _PT;
    Cell* _cell;
    Net* _net;
};

class Net {
   public:
    // Constructors(no copy constructor)
    Net(const std::string NetName,
        unsigned id,
        unsigned PinNum,
        unsigned layer);

    // modifier
    void addPin(Pin* pin);
    void addSegment();

    // accesser
    const std::string& getName() const;
    unsigned getId() const;
    unsigned getMinlayer() const; //min routing layer constraint
    size_t getNumPin() const;
    Pin& getPin(unsigned i);

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
         unsigned id);

    // modifier
    void setRow(unsigned x);
    void setColumn(unsigned y);
    void setCoordinate(unsigned row, unsigned column);
    void move(); // use this to specify that this cell is moved

    // accesser
    const std::string& getCellName() const;
    MasterCellType& getMasterCell();
    int getMasterCellId() const;
    bool movable(bool constraint) const;
    unsigned getRow() const;
    unsigned getColumn() const;
    Pin& getPin(size_t i);
    Pin& getPin(std::string& str);
    int getLayerDemand(int layer) const;
    safe::vector<unsigned>& getSameGridMC(unsigned layer);
    safe::vector<unsigned>& getadjHGridMC(unsigned layer);
    safe::vector<int>& getSameGridDemand(unsigned layer);
    safe::vector<int>& getadjHGridDemand(unsigned layer);
    size_t getNumPins() const;
    const safe::vector<Pin*>& getPinLayer(int i) const;
    safe::vector<Pin*>& getPinLayer(int i);

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
    safe::vector<safe::vector<Pin*>> _Layer2pin;
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);
