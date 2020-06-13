/***********************************************************************

  FileName    [Grid.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file defines the Layers, the Grids, and their Coordinates(to
  save cells).

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Cell.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/* Layer :
 *       Its main purpose is to manage the grids on this layer and the
 *    routing direction. So remember to look up the routing direction
 *    before routing.
 * 
 * Coordinate :
 *       Its main purpose is to manage the "add" and "remove" of cells.
 *    If you're routing, you probably don't need this class.
 * 
 * Grid :
 *       Although the class has a lot of data members, the only situation
 *    when you need this is when you "remove" or "add" a wire from or
 *    into the Grid. Otherwise, the data members are well managed by
 *    other classes.
*/

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Grid;

class Layer {
   public:
    // constructor
    Layer(const std::string name, int i, bool d, int supply, int area);

    // destructor
    ~Layer();

    // modifier

    // accesser
    const std::string& getLayerName() const;
    int getLayerIdx() const;
    Grid& getGrid(int i);

   private:
    const std::string _LayerName;
    const int _idx;
    bool _direction;  // 0 for H(column), 1 for V(row)
    safe::vector<Grid*> _grids;
};

class Coordinate {
   public:
    // constructor
    Coordinate(int x, int y, int layer);

    // modifier
    void addAdjH(Coordinate* c1, Coordinate* c2);  // you don't need this'
    void addGrid(Grid* g);                         // you don't need this
    bool CanAddCell(Cell& cell);  // to see if you can add this cell
    void addCell(Cell& cell);
    void moveCell(Cell& cell);

    // accesser
    Grid& getGrid(size_t i);
    int getRow() const;
    int getColumn() const;

   private:
    int _row;
    int _column;
    Coordinate* _c1;
    Coordinate* _c2;
    safe::vector<Grid*> _grids;

    void addConstraint(int layer,
                       safe::vector<unsigned>& mc,
                       safe::vector<int>& demand);
    void moveConstraint(int layer,
                        safe::vector<unsigned>& mc,
                        safe::vector<int>& demand);
};

class Grid {
   public:
    // constructors
    Grid(int supply, Layer& layer);
    Grid(Grid& a);

    // modifier
    void assignCoordinate(Coordinate* c);
    void incSupply(int d);
    void decSupply(int d);
    void addConstraint(unsigned mc, int demand);
    void moveConstraint(unsigned mc, int demand);
    bool CanAddCell(MasterCellType& mct);
    void addCell(MasterCellType& mct);
    void moveCell(MasterCellType& mct);
    void addNet(GridNet& net);
    bool getNet(GridNet& net);
    GridNet* getNet(unsigned i);

    // accesser
    int getRow() const;
    int getColumn() const;
    int getSupply() const;
    int getLayer() const;
    bool getDemand(unsigned mc, int& demand);

   private:
    int _supply;
    Layer& _layer;
    Coordinate* _coordinate;
    safe::unordered_map<unsigned, int> _Cell2Demand;
    safe::unordered_map<unsigned, GridNet*> _nets;
};
