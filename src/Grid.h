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

// TODO enum class Direction

/*
 * Layer :
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
    // FIXME why does pointer of vector survie
   public:
    // constructor
    Layer(const std::string name, int i, bool d, int supply, int area) noexcept;
    Layer(const Layer& l) noexcept;

    // destructor
    ~Layer();

    // modifier

    // accesser
    const std::string& getLayerName() const;
    int getLayerIdx() const;
    bool getDirection() const; // 0 for H(column), 1 for V(row)
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
    Coordinate(const Coordinate& c) noexcept;

    // modifier
    void addAdjH(Coordinate* c1, Coordinate* c2);  // you don't need this
    void addGrid(Grid* g);                         // you don't need this
    bool CanAddCell(Cell& cell) const;  // to see if you can add this cell
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
    safe::unordered_map<unsigned, unsigned> _MCT2Num;
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
    void addNet(GridNet& net);
    GridNet& getNet(unsigned i);
    void rmNet(GridNet& net);
    void rmNet(unsigned i);

    // accesser
    int getRow() const;
    int getColumn() const;
    int getSupply() const;
    int getLayer() const;
    bool canGetNet(const GridNet& net) const;
    bool canGetNet(unsigned i) const;

   private:
    int _supply;
    Layer& _layer;
    Coordinate* _coordinate;
    safe::unordered_map<unsigned, GridNet*> _nets;
};
