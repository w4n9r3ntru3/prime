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
    //constructor
    Layer(const std::string name, int i, bool d, int supply, int area):_direction(d), _LayerName(name), _idx(i)
    {
        _grids.reserve(area);
        for (int i = 0; i < area; ++i) _grids.push_back(Grid(supply, this*));
    }

    //accesser
    std::string& getLayerName() const       { return _LayerName; }

private:
    const std::string           _LayerName;
    const int                   _idx;
    bool                        _direction; //0 for H(column), 1 for V(row)
    std::vector<Grid>           _grids;
};

class Coordinate
{
public:
    //constructor
    Coordinate(int x, int y, int layer):_row(x),_column(y)
    {
        _grids.reserve(layer);
    }

    //modifier
    void addGrid(Grid* g)                   { _grids.push_back(g); }
    void addCell(Cell* c)                   { _cells.push_back(c); }

    //accesser
    Cell& getCell(size_t i)                 { return *_cells[i]; }
    Grid& getGrid(size_t i)                 { return *_grids[i]; }
    int getRow() const                      { return _row; }
    int getColumn() const                   { return _column; }

private:
    int                         _row;
    int                         _column;
    std::vector<Cell*>          _cells;
    std::vector<Grid*>          _grids;
};

class Grid
{
public:
    //constructors
    Grid(int supply, Layer& layer):_layer(layer), _supply(supply) {}
    Grid(Grid& a):_layer(a._layer), _supply(a._supply), _coordinate(a._coordinate) {}

    //modifier
    void assignCoordinate(Coordinate& c)    { _coordinate = c; }
    void incSupply(int d)                   { _supply += d; }

    //accesser
    int getRow() const                      { return _coordinate.getRow(); }
    int getColumn() const                   { return _coordinate.getColumn(); }
    int getLayer() const                    { return _layer; }
    int getSupply() const                   { return _supply; }
private:
    int                         _supply;
    Coordinate&                 _coordinate;
    Layer&                      _layer;
};

#endif