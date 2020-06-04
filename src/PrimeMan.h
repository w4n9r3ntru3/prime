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
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class PrimeMan {
   public:
    // Constructor
    PrimeMan(std::fstream& input);
    // destructor
    ~PrimeMan();

    // accesser(get -1 if out of bound)
    int getIdx(int row, int column) const {
        assert(column >= 0 && column < _columnRange && row >= 0 &&
               row < _rowRange);
        return column * _rowRange + row;
    }
    int getLeft(int row, int column) const {
        if (column == 0) {
            return -1;
        }
        return getIdx(row, column - 1);
    }
    int getRight(int row, int column) const {
        if (column == _columnRange - 1) {
            return -1;
        }
        return getIdx(row, column + 1);
    }
    int getDown(int row, int column) const {
        if (row == 0) {
            return -1;
        }
        return getIdx(row - 1, column);
    }
    int getUp(int row, int column) const {
        if (row == _rowRange - 1) {
            return -1;
        }
        return getIdx(row + 1, column);
    }

    Layer& getLayer(int layer) {
        assert(layer < _layers.size());
        return *_layers[layer];
    }
    Coordinate& getCoordinate(unsigned i) {
        assert(i >= 0 && i < _coordinates.size());
        return *_coordinates[i];
    }
    Cell& getCell(unsigned i) {
        assert(i < _cells.size());
        return *_cells[i];
    }
    Net& getNet(unsigned i) {
        assert(i < _nets.size());
        return *_nets[i];
    }

   private:
    unsigned _maxMove;
    int _rowBase;
    int _columnBase;
    int _rowRange;
    int _columnRange;
    int _area;
    int _layer;
    std::unordered_map<std::string, int> _Layer2Idx;
    std::unordered_map<std::string, int> _MasterCell2Idx;
    std::unordered_map<std::string, unsigned> _Cell2Idx;
    std::unordered_map<std::string, unsigned> _Net2Idx;
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
