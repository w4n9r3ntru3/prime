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
#include "../include/safe.h"

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
 *  You don't need to update the position of a pin but if you want the
 *  of a pin, you need to get the idx of the cell to which it belongs
 *  first and get the position from the cell.
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

class Pin {
   public:
    // constructor
    Pin(unsigned idx, unsigned layer, Cell& cell);
    Pin(const Pin& pin) = delete;
    Pin(Pin&& pin);

    // Assignment
    Pin& operator=(const Pin& pin) = delete;
    Pin& operator=(Pin&& pin);

    // modifier
    void setNet(GridNet& net);

    // accesser
    unsigned getIdx() const;
    unsigned get_net_idx() const;
    unsigned get_cell_idx() const;
    unsigned getLayer() const;

    //

   private:
    unsigned _idx;
    unsigned _layer;
    unsigned _cell;
    unsigned _net;
};

class GridNet {
   public:
    // Constructors(no copy constructor)
    GridNet(unsigned idx,
            unsigned PinNum,
            unsigned minLayer);
    GridNet(const GridNet& g) = delete;
    GridNet(GridNet&& g);

    // Assignment
    GridNet& operator=(const GridNet& net) = delete;
    GridNet& operator=(GridNet&& net);

    // modifier
    void addPin(Pin& pin);  // you don't need this
    // void addSegment(int srow, int scol, int slay, int erow, int ecol, int
    // elay);

    // accesser
    unsigned getIdx() const;
    unsigned getMinlayer() const;  // min routing layer constraint
    size_t getNumPin() const;
    Pin& getPin(unsigned i, safe::vector<Cell>& cells);
    // size_t getNumSegments() const;
    // safe::vector<unsigned>& getSegments();

   private:
    unsigned _idx;
    unsigned _minLayer;
    safe::vector<std::pair<unsigned, unsigned>> _pins;

    // ! TODO deprecate this
    // safe::vector<unsigned> _segments;  // srow, scol, slay, erow, ecol, elay
};

class Cell {
   public:
    // Constructors(no copy constructor)
    Cell(MasterCellType& MCT,
         unsigned idx,
         bool movable,
         unsigned layers);
    Cell(const Cell& c) = delete;
    Cell(Cell&& c);

    // Assignment
    Cell& operator=(const Cell& c) = delete;
    Cell& operator=(Cell&& c);

    // modifier
    void setRow(unsigned x);
    void setColumn(unsigned y);
    void setCoordinate(unsigned row, unsigned column);
    void move();  // use this to specify that this cell is moved

    // accesser
    const std::string& getCellName() const;
    unsigned getIdx() const;
    const MasterCellType& getMasterCell() const;
    MasterCellType& getMasterCell();
    unsigned getMasterCellId() const;

    bool moved() const;
    // constraint means the limit of movable number
    bool movable(bool constraint) const;

    unsigned getRow() const;
    unsigned getColumn() const;

    Pin& getPin(unsigned i);

    int getLayerDemand(unsigned layer) const;
    safe::vector<unsigned>& getSameGridMC(unsigned layer);
    safe::vector<unsigned>& getadjHGridMC(unsigned layer);
    safe::vector<int>& getSameGridDemand(unsigned layer);
    safe::vector<int>& getadjHGridDemand(unsigned layer);

    size_t getNumPins() const;

    const safe::vector<unsigned>& getPinLayer(unsigned i) const;

    // friend
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

   private:
    MasterCellType& _MCT;
    unsigned _idx;
    bool _movable;
    bool _moved;
    unsigned _row;
    unsigned _column;
    safe::vector<Pin> _pins;
    safe::vector<safe::vector<unsigned>> _Layer2pin;
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);// to be updated
