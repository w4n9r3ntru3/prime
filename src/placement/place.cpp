/***********************************************************************

  FileName    [Grid.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file defines the Layers, the Grids, and their Coordinates(to
  save cells).

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "place.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           FUNCTIONS                              ///
////////////////////////////////////////////////////////////////////////

BoundingNet::BoundingNet(Chip& chp, GridNet& net)
    : _leftmost(chp.getNumColumns() - 1),
      _leftnext(chp.getNumColumns() - 1),
      _rightmost(0),
      _rightnext(0),
      _topmost(0),
      _topnext(0),
      _bottommost(chp.getNumRows() - 1),
      _bottomnext(chp.getNumRows() - 1) {
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        const Pin& pin = chp.getPin(net.getPinIdx(i));
        unsigned row = chp.getPinRow(pin), column = chp.getPinColumn(pin);
        updatePos(row, column, pin.getIdx());
    }
}

BoundingNet::BoundingNet(BoundingNet&& other)
    : _leftmost(other._leftmost),
      _leftnext(other._leftnext),
      _rightmost(other._rightmost),
      _rightnext(other._rightnext),
      _topmost(other._topmost),
      _topnext(other._topnext),
      _bottommost(other._bottommost),
      _bottomnext(other._bottomnext),
      _leftmostpin(other._leftmostpin),
      _rightmostpin(other._rightmostpin),
      _topmostpin(other._topmostpin),
      _bottommostpin(other._bottommostpin) {}

void BoundingNet::updatePos(const unsigned row,
                            const unsigned column,
                            const unsigned pin) {
    if (column < _leftmost) {
        _leftnext = _leftmost;
        _leftmost = column;
        _leftmostpin = pin;
    } else if (column < _leftnext) {
        _leftnext = column;
    }

    if (column > _rightmost) {
        _rightnext = _rightmost;
        _rightmost = column;
        _rightmostpin = pin;
    } else if (column > _rightnext) {
        _rightnext = column;
    }

    if (row < _bottommost) {
        _bottomnext = _bottommost;
        _bottommost = row;
        _bottommostpin = pin;
    } else if (row < _bottomnext) {
        _bottomnext = row;
    }

    if (row > _topmost) {
        _topnext = _topmost;
        _topmost = row;
        _topmostpin = pin;
    } else if (row > _topnext) {
        _topnext = row;
    }
}

Place::Place(Chip& chp) : _chp(chp) {
    _nets.reserve(_chp.getNumNets());
    for (unsigned i = 0; i < _chp.getNumNets(); ++i) {
        _nets.push_back(std::move(BoundingNet(_chp, _chp.getNet(i))));
    }
}