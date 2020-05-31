/***********************************************************************

  FileName    [Grid.h]

  Author      [Yang, Chien Yi]

  This file defines the Layers, the Grids, and their Coordinates(to
  save cells).

***********************************************************************/

#ifndef GRID_H
#define GRID_H

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Cell.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Grid;

class Layer
{
public:
private:
};

class Coordinate
{
public:
private:

};

class Grid
{
public:
    //constructors
    Grid(unsigned i, int supply, Coordinate& c):_idx(i), _supply(supply), _coordinate(c) {}
    Grid(Grid& a):_idx(a._idx), _supply(a._supply), _coordinate(a._coordinate) {}
private:
    unsigned                    _idx;
    int                         _supply;
    Coordinate&                 _coordinate;
};

#endif