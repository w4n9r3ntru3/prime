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
#include "QuadTree.h"
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
 * 3. Do the following steps when you move a cell :
 *    a. Use Cell::movable(Chip::limited()) to check if you can move
 *       the cell.
 *    b. Get the Coordinate of the position into which you want to move
 *       the cell and use Coordinate::CanAddCell() to check if it's
 *       legal.
 *    c. Get the Coordinate of the original position and use
 *       Coordinate::moveCell().
 *    d. Get the new Coordinate and use Coordinate::addCell().
 *    e. Call Chip::moveCell().
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
    int getIdx(int row, int column) const;
    int getLeft(int row, int column) const;
    int getRight(int row, int column) const;
    int getDown(int row, int column) const;
    int getUp(int row, int column) const;

    // modifier
    void moveCell(Cell& cell);
    void decNumMoved();  // trigger it if you want to move a cell back to its
                         // original position

    // accesser(use getIdx() to get the idx of Grid and Coordinate)
    size_t getNumLayers() const;
    size_t getNumColumns() const;
    size_t getNumRows() const;
    size_t getArea() const;
    size_t getVolume() const;
    size_t getNumNets() const;
    size_t getNumCells() const;
    size_t getNumMasterCells() const;
    Layer& getLayer(int layer);
    Coordinate& getCoordinate(unsigned idx);
    Cell& getCell(unsigned idx);
    GridNet& getNet(unsigned idx);
    Grid& getGrid(int layer, unsigned idx);
    Grid& getGrid(int layer, int row, int column);
    MasterCellType& getMasterCell(unsigned idx);
    bool limited() const;
    void log() const;

    // output
    // void output(std::fstream& output);

   private:
    unsigned _maxMove;
    int _rowBase;
    int _columnBase;
    int _rowRange;
    int _columnRange;
    int _area;
    int _layer;
    safe::unordered_map<std::string, int> _Layer2Idx;
    safe::unordered_map<std::string, int> _MasterCell2Idx;
    safe::unordered_map<std::string, unsigned> _Cell2Idx;
    safe::unordered_map<std::string, unsigned> _Net2Idx;
    safe::vector<std::unique_ptr<Layer>> _layers;
    safe::vector<std::shared_ptr<Coordinate>> _coordinates;
    safe::vector<MasterCellType> _MasterCells;
    safe::vector<std::unique_ptr<Cell>> _cells;

    // !
    safe::vector<GridNet> _grid_nets;
    // FIXME this should not be placed here:
    safe::vector<QuadTree> _quad_tree_nets;

    // ! substituted
    // safe::vector<TreeNet> _tree_nets;

    safe::vector<unsigned> _movedCells;

    // private function
    void readFile(std::fstream& input);
    void constructCoordinate();
    void constructGrid(int layer);
    void connectCoordinateGrid();
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
