/***********************************************************************

  FileName    [PrimeMan.h]

  Author      [Yang Chien Yi]

  This file difines the data manager for prime.

***********************************************************************/

#ifndef PRIMEMAN_H
#define PRIMEMAN_H

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include<fstream>

#include "Cell.h"
#include "Grid.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class PrimeMan
{
public:
    //Constructor
    PrimeMan(std::fstream& input);
    //destructor
    ~PrimeMan();

    //accesser(get -1 if out of bound)
    int getIdx(int row, int column) const               { assert(column >= 0 && column < _columnRange && row >= 0 && row < _rowRange); return column*_rowRange+row; }
    int getLeft(int row, int column) const              { if(column == 0) return -1; return getIdx(row,column-1); }
    int getRight(int row, int column) const             { if(column == _columnRange-1) return -1; return getIdx(row,column+1); }
    int getDown(int row, int column) const              { if(row == 0) return -1; return getIdx(row-1,column); }
    int getUp(int row, int column) const                { if(row == _rowRange-1) return -1; return getIdx(row+1,column); }

private:
    unsigned                                    _maxMove;
    int                                         _rowBase;
    int                                         _columnBase;
    int                                         _rowRange;
    int                                         _columnRange;
    int                                         _area;
    int                                         _layer;
    std::unordered_map<std::string,int>         _Layer2Idx;
    std::unordered_map<std::string,int>         _MasterCell2Idx;
    std::unordered_map<std::string,unsigned>    _Cell2Idx;
    std::unordered_map<std::string,unsigned>    _Net2Idx;
    std::vector<Layer*>                         _layers;
    std::vector<Coordinate*>                    _coordinates;
    std::vector<MasterCellType*>                _MasterCells;
    std::vector<Cell*>                          _cells;
    std::vector<Net*>                           _nets;

    //private function
    void readFile(std::fstream& input);
    void constructCoordinate();
    void constructGrid(int layer);
    void connectCoordinateGrid();
};

#endif