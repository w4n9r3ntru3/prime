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
    Layer(unsigned idx, bool d, int supply, unsigned area);
    Layer(const Layer& l) = delete;
    Layer(Layer&& l);

    // Assignment
    Layer& operator=(const Layer& l) = delete;
    Layer& operator=(Layer&& l);

    // modifier

    // accesser
    unsigned getLayerIdx() const;
    bool getDirection() const;  // 0 for H(column), 1 for V(row)
    Grid& getGrid(unsigned idx);

   private:
    unsigned _idx;
    bool _direction;  // 0 for H(column), 1 for V(row)
    safe::vector<Grid> _grids;
};

class Coordinate {
   public:
    // constructor
    Coordinate(unsigned x, unsigned y, unsigned idx, int left, int right);
    Coordinate(const Coordinate& c) = delete;
    Coordinate(Coordinate&& c);

    // Assignment
    Coordinate& operator=(const Coordinate& c) = delete;
    Coordinate& operator=(Coordinate&& c);

    // modifier
    bool CanAddCell(
        Cell& cell,
        safe::vector<Coordinate>& coordinates,
        safe::vector<Layer>& layers) const;  // to see if you can add this cell
    void initCell(Cell& cell,
                 safe::vector<Coordinate>& coordinates,
                 safe::vector<Layer>& layers);
    void addCell(Cell& cell,
                 safe::vector<Coordinate>& coordinates,
                 safe::vector<Layer>& layers,
                 safe::vector<Pin>& pins);
    void rmCell(Cell& cell,
                safe::vector<Coordinate>& coordinates,
                safe::vector<Layer>& layers,
                safe::vector<Pin>& pins);

    // accesser
    int getRow() const;
    int getColumn() const;

   private:
    unsigned _row;
    unsigned _column;
    unsigned _idx;
    int _c1;
    int _c2;
    safe::unordered_map<unsigned, unsigned> _MCT2Num;
};

class Grid {
   public:
    // constructors
    Grid(int supply);
    Grid(const Grid& g) = delete;
    Grid(Grid&& g);

    // Assignment
    Grid& operator=(const Grid& g) = delete;
    Grid& operator=(Grid&& g);

    // modifier
    void incSupply(int d);
    void decSupply(int d);
    void addPin(Pin& pin);
    void addPin(int i);
    void addPin(const safe::vector<unsigned>& idx, safe::vector<Pin>& pins);
    void rmPin(Pin& pin);
    void rmPin(int i);
    void rmPin(const safe::vector<unsigned>& idx, safe::vector<Pin>& pins);
    void addNet(GridNet& net);
    void addNet(unsigned i);
    void rmNet(GridNet& net);
    void rmNet(unsigned i);
    GridNet& getNet(unsigned i, std::vector<GridNet>& nets);

    // accesser
    int getSupply() const;
    bool canGetNet(const GridNet& net) const;
    bool canGetNet(unsigned i) const;
    bool routable(const GridNet& net) const;

   private:
    int _supply;
    safe::unordered_map<unsigned, unsigned> _pins;
    safe::unordered_set<unsigned> _nets;
};
