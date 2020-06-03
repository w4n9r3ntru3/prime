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

    //accesser
    int getIdx(int row, int column) const               { return column*_rowRange+row; }

private:
    unsigned                                _maxMove;
    int                                     _rowBase;
    int                                     _columnBase;
    int                                     _rowRange;
    int                                     _columnRange;
    int                                     _area;
    int                                     _layer;
    std::unordered_map<std::string,int>     _Layer2Idx;
    std::unordered_map<unsigned,int>        _MasterCell2Idx;
    std::vector<Layer*>                     _layers;
    std::vector<Coordinate*>                _coordinates;
    std::vector<MasterCellType*>            _MasterCells;

    //private function
    void readFile(std::fstream& input);
    void constructCoordinate();
    void constructGrid(int layer);
    void connectCoordinateGrid();
};

#endif