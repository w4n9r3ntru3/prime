/***********************************************************************

  FileName    [PrimeMan.h]

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

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/* 1. How to get elements :
 *    Cell : getCell()
 *    Net : getNet()
 *    Coordinate : getCoordinate()
 *    Layer : getLayer()
 *    Grid : getGrid()
 *    Pin : getCell() first and then getPin() from the Cell
 *          getNet first and then getPin() from the Net.
 *
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

class PrimeMan {
   public:
    // Constructor
    PrimeMan(std::fstream& input);
    // destructor
    ~PrimeMan();

    // index(return -1 if out of bound)
    int getIdx(int row, int column) const;
    int getLeft(int row, int column) const;
    int getRight(int row, int column) const;
    int getDown(int row, int column) const;
    int getUp(int row, int column) const;

    // modifier
    bool incNumMoved();  // trigger it if you want to move a cell
    void decNumMoved();  // trigger it if you want to move a cell back to its original
                         // position

    // accesser(use getIdx() to get the idx of Grid and Coordinate)
    Layer& getLayer(int layer);
    Coordinate& getCoordinate(unsigned idx);
    Cell& getCell(unsigned idx);
    Net& getNet(unsigned idx);
    Grid& getGrid(int layer, unsigned idx);
    bool limited() const;

   private:
    unsigned _maxMove;
    unsigned _numMoved;
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
    safe::vector<Layer*> _layers;
    safe::vector<Coordinate*> _coordinates;
    safe::vector<MasterCellType*> _MasterCells;
    safe::vector<Cell*> _cells;
    safe::vector<Net*> _nets;

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
                     Net* net);
};
