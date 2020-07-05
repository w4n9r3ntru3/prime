/***********************************************************************

  FileName    [Chip.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file difines the data manager for prime.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <fstream>

#include "Cell.h"
#include "Grid.h"
//#include "QuadTree.h"
// #include "TreeNet.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/*
 * Which ".h" files does the classes belong to?
 * Cell.h : GridNet, Cell, Pin
 * Grid.h : Layer, Coordinate, Grid
 * MasterCell.h : PinType, BlockageType(probably useless), MasterCellType
 *
 * 1. How to get elements :
 *    Cell : getCell()
 *    Net : getNet()
 *    Coordinate : getCoordinate()
 *    Layer : getLayer()
 *    Grid : getGrid()
 *    Pin : getCell() first and then getPin() from the Cell
 *          getNet() first and then getPin() from the Net.
 *
 ! FIXME does not apply to all Net classes
 * 2. How to get the route of a Net :
 *    Get a Pin from the Net then traverse through the Grids by
 *    searching if the Net is in the adjacent Grids from the pin and
 *    recursively search for the Grids of the Net. Note that there are
 *    2 constraints when searching for the Grids: 1) Routing direction
 *    constraint and 2) Min Routing layer constraint
 *
 * Q&A : (If you have any question, please directly add the question in
 *        this Q&A and make a sign in the commit message. Then I'll
 *        answer it ASAP.)
 * Q: What is Coordinate?
 * A: Coordinate is the well in which you put a cell. However, you can't
 *    directly see which cells are in the Coordinate from itself but you
 *    can get the cost if you remove a cell from it or add a cell into
 *    it.
 */
////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Chip {
   public:
    // Constructor
    Chip(std::fstream& input);
    // destructor
    ~Chip();

    // index(return -1 if out of bound)
    int getIdx(unsigned row, unsigned column) const;
    int getLeft(unsigned row, unsigned column) const;
    int getRight(unsigned row, unsigned column) const;
    int getDown(unsigned row, unsigned column) const;
    int getUp(unsigned row, unsigned column) const;

    // modifier
    bool moveCell(Cell& cell, unsigned origin, unsigned target);
    void decNumMoved();  // trigger it if you want to move a cell back to its
                         // original position

    // accesser(use getIdx() to get the idx of Grid and Coordinate)
    size_t getNumLayers() const;
    size_t getColumnBase() const;
    size_t getRowBase() const;
    size_t getNumColumns() const;
    size_t getNumRows() const;
    size_t getArea() const;
    size_t getVolume() const;
    size_t getNumNets() const;
    size_t getNumCells() const;
    size_t getNumMasterCells() const;
    size_t getNumPins() const;
    Layer& getLayer(int layer);
    Coordinate& getCoordinate(unsigned idx);
    Cell& getCell(unsigned idx);
    const Pin& getPin(GridNet& net, unsigned idx);
    unsigned getPinRow(const Pin& pin) const;
    unsigned getPinColumn(const Pin& pin) const;
    GridNet& getNet(unsigned idx);
    Grid& getGrid(unsigned layer, unsigned idx);
    Grid& getGrid(unsigned layer, unsigned row, unsigned column);
    MasterCellType& getMasterCell(unsigned idx);
    bool limited() const;
    void log() const;

    // output
    // void output(std::fstream& output);

   private:
    unsigned _maxMove;
    unsigned _rowBase;
    unsigned _columnBase;
    unsigned _rowRange;
    unsigned _columnRange;
    unsigned _area;
    unsigned _layer;
    safe::vector<Layer> _layers;
    safe::vector<Coordinate> _coordinates;
    safe::vector<MasterCellType> _MasterCells;
    safe::vector<Cell> _cells;

    // !
    safe::vector<GridNet> _grid_nets;
    // TODO:
    //safe::vector<QuadTree> _quad_tree_nets;

    // ! substituted
    // safe::vector<TreeNet> _tree_nets;

    safe::vector<unsigned> _movedCells;

    // private function
    void readFile(std::fstream& input);
    void constructCoordinate();
    void constructGrid(int layer);
    void assignRoute(int srow,
                     int scol,
                     int slay,
                     int erow,
                     int ecol,
                     int elay,
                     GridNet& net);
    // void outputRoute(std::fstream& output);
    void maxNetDegree() const;
};

unsigned str2Idx(std::string title, std::string& str);