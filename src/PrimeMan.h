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
      Cell : getCell()
      
 *
 *
 *
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

    // accesser
    Layer& getLayer(int layer);
    Coordinate& getCoordinate(unsigned idx); // use getIdx() to get the idx
    Cell& getCell(unsigned idx);
    Net& getNet(unsigned idx);

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
