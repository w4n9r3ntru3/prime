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

#include <climits>

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

unsigned BoundingNet::getLeftmost() const {
    return _leftmostpin;
}

unsigned BoundingNet::getRightmost() const {
    return _rightmostpin;
}

unsigned BoundingNet::getTopmost() const {
    return _topmostpin;
}

unsigned BoundingNet::getBottommost() const {
    return _bottommostpin;
}

unsigned BoundingNet::getLeftRange() const {
    assert(_leftnext >= _leftmost);
    return _leftnext - _leftmost;
}
unsigned BoundingNet::getRightRange() const {
    assert(_rightnext <= _rightmost);
    return _rightmost - _rightnext;
}
unsigned BoundingNet::getUpRange() const {
    assert(_topnext <= _topmost);
    return _topmost - _topnext;
}
unsigned BoundingNet::getDownRange() const {
    assert(_bottomnext >= _bottommost);
    return _bottomnext - _bottommost;
}

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

moveCell::moveCell()
    : _H(0),
      _V(0),
      _rightRange(UINT_MAX),
      _leftRange(UINT_MAX),
      _upRange(UINT_MAX),
      _downRange(UINT_MAX) {}

moveCell::moveCell(moveCell&& other)
    : _H(other._H),
      _V(other._V),
      _rightRange(other._rightRange),
      _leftRange(other._leftRange),
      _upRange(other._upRange),
      _downRange(other._downRange) {}

void moveCell::setleftRange(const unsigned range) {
    _H--;
    if (range < _leftRange) {
        _leftRange = range;
    }
}

void moveCell::setrightRange(const unsigned range) {
    _H++;
    if (range < _rightRange) {
        _rightRange = range;
    }
}

void moveCell::setupRange(const unsigned range) {
    _V++;
    if (range < _upRange) {
        _upRange = range;
    }
}

void moveCell::setdownRange(const unsigned range) {
    _V--;
    if (range < _downRange) {
        _downRange = range;
    }
}

int moveCell::getHgain() const {
    return _H > 0 ? abs(_H) * _rightRange : abs(_H) * _leftRange;
}

int moveCell::getVgain() const {
    return _V > 0 ? abs(_V) * _rightRange : abs(_V) * _leftRange;
}

int moveCell::getH() const {
    return _H;
}

int moveCell::getV() const {
    return _V;
}

unsigned moveCell::getRightRange() const {
    return _rightRange;
}

unsigned moveCell::getLeftRange() const {
    return _leftRange;
}

unsigned moveCell::getDownRange() const {
    return _downRange;
}

unsigned moveCell::getUpRange() const {
    return _upRange;
}

Place::Place(Chip& chp) : _chp(chp) {
    _cells.reserve(_chp.getNumCells());
    _nets.reserve(_chp.getNumNets());
    for (unsigned i = 0; i < _chp.getNumCells(); ++i) {
        _cells.push_back(std::move(moveCell()));
    }
    for (unsigned i = 0; i < _chp.getNumNets(); ++i) {
        _nets.push_back(std::move(BoundingNet(_chp, _chp.getNet(i))));
        updateCell(i);
    }
    // move
    argList list;
    for (unsigned i = 0; i < _chp.getNumCells(); ++i) {
        if (_chp.getCell(i).movable(_chp.limited())) {
            moveCell& cell = _cells[i];
            list.push_back(
                std::make_pair(i, cell.getHgain() + cell.getVgain()));
        }
    }
    std::sort(list.begin(), list.end(), myfunc);
    for (unsigned i = 0; i < _chp.getMaxMove(); ++i) {
        auto j = list[i];
        if (j.second == 0) {
            break;
        }
        unsigned idx = j.first;
        Cell& cell = _chp.getCell(idx);
        moveCell& m_cell = _cells[idx];
        unsigned erow = 0, ecol = 0;
        if (m_cell.getH() > 0) {
            ecol = cell.getColumn() + m_cell.getRightRange();
        } else if (m_cell.getH() < 0) {
            ecol = cell.getColumn() - m_cell.getLeftRange();
        }
        if (m_cell.getV() > 0) {
            erow = cell.getRow() + m_cell.getUpRange();
        } else if (m_cell.getV() < 0) {
            erow = cell.getRow() - m_cell.getDownRange();
        }
        _chp.moveCell(cell, cell.getRow(), cell.getColumn(), erow, ecol);
    }
}

inline void Place::updateCell(const unsigned i) {
    BoundingNet& net = _nets[i];
    unsigned cell;
    cell = _chp.getPin(net.getLeftmost()).get_cell_idx();
    Cell& c1 = _chp.getCell(cell);
    assert(c1.getColumn() >= net.getLeftRange());
    _cells[cell].setleftRange(net.getLeftRange());

    cell = _chp.getPin(net.getRightmost()).get_cell_idx();
    Cell& c2 = _chp.getCell(cell);
    assert(c2.getColumn() + net.getRightRange() < _chp.getNumColumns());
    _cells[cell].setrightRange(net.getRightRange());

    cell = _chp.getPin(net.getTopmost()).get_cell_idx();
    Cell& c3 = _chp.getCell(cell);
    assert(c3.getRow() + net.getUpRange() < _chp.getNumRows());
    _cells[cell].setupRange(net.getUpRange());

    cell = _chp.getPin(net.getBottommost()).get_cell_idx();
    Cell& c4 = _chp.getCell(cell);
    assert(c4.getRow() >= net.getDownRange());
    _cells[cell].setdownRange(net.getDownRange());
}

bool myfunc(std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b) {
    return a.second > b.second;
}