/***********************************************************************

  FileName    [Grid.h]

  Author      [Yang Chien Yi]

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
        for (int i = 0; i < area; ++i) { Grid* g = new Grid(supply, *this); _grids.push_back(g); }
    }

    //destructor
    ~Layer() { for (int i = 0, n = _grids.size(); i < n; ++i) delete _grids[i]; }

    //modifier

    //accesser
    const std::string& getLayerName() const         { return _LayerName; }
    int getLayerIdx() const                         { return _idx; }
    Grid& getGrid(int i)                            { return *_grids[i]; }

private:
    const std::string           _LayerName;
    const int                   _idx;
    bool                        _direction; //0 for H(column), 1 for V(row)
    std::vector<Grid*>           _grids;
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
    void addGrid(Grid* g)                           { _grids.push_back(g); }
    void addCell(Cell* cell, Coordinate* c1, Coordinate* c2)
    {
        //for every layer
        for(int i = 0, n = _grids.size(); i < n; ++i)
        {
            _grids[i]->addCell(cell->getMasterCellId());
            std::vector<unsigned> adjHMC = cell->getadjHGridMC(i);
            std::vector<unsigned> SameMC = cell->getSameGridMC(i);
            std::vector<int> adjHDemand = cell->getadjHGridDemand(i);
            std::vector<int> SameDemand = cell->getSameGridDemand(i);
            addConstraint(i,SameMC,SameDemand);
            if(c1) c1->addConstraint(i,adjHMC,adjHDemand);
            if(c2) c2->addConstraint(i,adjHMC,adjHDemand);
        }
    }
    void addConstraint(int layer, std::vector<unsigned>& mc, std::vector<int>& demand)
    {
        assert(mc.size() == demand.size());
        for(int i = 0, n = mc.size(); i < n; ++i) _grids[layer]->addConstraint(mc[i], demand[i]);
    }

    //accesser
    Grid& getGrid(size_t i)                         { return *_grids[i]; }
    int getRow() const                              { return _row; }
    int getColumn() const                           { return _column; }

private:
    int                         _row;
    int                         _column;
    std::vector<Grid*>          _grids;
};

class Grid
{
public:
    //constructors
    Grid(int supply, Layer& layer):_layer(layer), _supply(supply) {}
    Grid(Grid& a):_layer(a._layer), _supply(a._supply), _coordinate(a._coordinate) {}

    //modifier
    void assignCoordinate(Coordinate* c)            { _coordinate = c; }
    void incSupply(int d)                           { _supply += d; }
    void decSupply(int d)                           { _supply -= d; }
    void addConstraint(unsigned mc, int demand)
    {
        if(_Cell2Demand.count(mc) == 0) _Cell2Demand[mc] = demand;
        else _Cell2Demand[mc] += demand;
    }
    void moveConstraint(unsigned mc, int demand)
    {
        assert(_Cell2Demand.count(mc) > 0);
        demand = _Cell2Demand[mc] - demand;
        if(demand > 0) _Cell2Demand[mc] = demand;
        else _Cell2Demand.erase(mc);
    }
    void addCell(unsigned mc)
    {
        int demand;
        if(getDemand(mc,demand)) _supply -= demand;
    }

    //accesser
    int getRow() const                              { return _coordinate->getRow(); }
    int getColumn() const                           { return _coordinate->getColumn(); }
    int getSupply() const                           { return _supply; }
    int getLayer() const                            { return _layer.getLayerIdx(); }
    bool getDemand(unsigned mc, int& demand)
    {
        if(_Cell2Demand.count(mc) == 0) return false;
        else demand = _Cell2Demand[mc];
        return true;
    }

private:
    int                                     _supply;
    Layer&                                  _layer;
    Coordinate*                             _coordinate;
    std::unordered_map<unsigned,int>        _Cell2Demand;
};

#endif