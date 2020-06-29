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
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/*
 * Cell :
 *      It is the class that describes a cell. It contains the following
 *  datas, master cell type, row, column, and its pins. Also, you can
 *  also look up if the cell is movable by calling movable().
 *
 * Pin :
 *      It is the class that describes a pin. It contains the information
 *  about its pin type, the cell it belongs to, and the net it belongs to.
 *  You don't need to update the position of a pin because you can get
 *  its position with the cell and the net it belongs to.
 *
 * Net :
 *      The structure of a GridNet is not yet determined. However, the
 *  basic structure still exists. It contains the segments of the route
 *  and the pins. It also contains the ||MIN LAYER ROUTING CONSTRAINT||.
 */

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class GridNet;
class Cell;

// * using pointers instead of references may seem like a downgrad
// * but move semantics are needed
class Pin {
   public:
    // constructor
    Pin(PinType& PT, Cell& cell);
    Pin(Pin&& p);

    // assignment
    Pin& operator=(Pin&& p);

    // modifier
    void setNet(std::shared_ptr<GridNet> net);

    // accesser
    PinType& getPinType() const;
    GridNet& get_net() const;
    Cell& get_cell() const;

    unsigned getRow() const;
    unsigned getColumn() const;
    int getLayer() const;

   private:
    std::shared_ptr<PinType> _PT;
    std::shared_ptr<Cell> _cell;
    std::shared_ptr<GridNet> _net;
};

class GridNet {
   public:
    // Constructors(no copy constructor)
    GridNet(const std::string NetName,
            unsigned id,
            unsigned PinNum,
            unsigned layer) noexcept;
    GridNet(const GridNet& g) noexcept;

    // modifier
    void addPin(std::shared_ptr<Pin> pin);  // you don't need this
    // void addSegment(int srow, int scol, int slay, int erow, int ecol, int
    // elay);

    // accesser
    const std::string& getName() const;
    unsigned getId() const;
    unsigned getMinlayer() const;  // min routing layer constraint
    size_t getNumPin() const;
    Pin& getPin(unsigned i);
    // size_t getNumSegments() const;
    // safe::vector<unsigned>& getSegments();

   private:
    const std::string _NetName;
    const unsigned _Id;
    const unsigned _minLayer;

    safe::vector<std::shared_ptr<Pin>> _pins;

    // ! TODO deprecate this
    // safe::vector<unsigned> _segments;  // srow, scol, slay, erow, ecol, elay
};

class Cell {
   public:
    // Constructors(no copy constructor)
    Cell(const std::string CellName,
         MasterCellType& MCT,
         bool movable,
         unsigned id) noexcept;
    Cell(Cell&& c) noexcept;

    // operator=
    Cell& operator=(Cell&& c) noexcept;

    // modifier
    void setRow(unsigned x);
    void setColumn(unsigned y);
    void setCoordinate(unsigned row, unsigned column);
    void move();  // use this to specify that this cell is moved

    // accesser
    const std::string& getCellName() const;
    unsigned getId() const;

    const MasterCellType& getMasterCell() const;
    MasterCellType& getMasterCell();

    int getMasterCellId() const;

    bool moved() const;
    // constraint means the limit of movable number
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

    const safe::vector<std::shared_ptr<Pin>>& getPinLayer(int i) const;
    safe::vector<std::shared_ptr<Pin>>& getPinLayer(int i);

    // friend
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

   private:
    std::string _CellName;
    MasterCellType& _MCT;

    unsigned _Id;
    bool _movable;
    bool _moved;
    unsigned _row;
    unsigned _column;

    safe::vector<Pin> _pins;
    safe::vector<safe::vector<std::shared_ptr<Pin>>> _Layer2pin;
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);
