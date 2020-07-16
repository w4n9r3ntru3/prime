/***********************************************************************

  FileName    [Force.cpp]

  Author      [Yang Chien Yi]

  This file describes the force-directed method.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Force.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/*

 */

////////////////////////////////////////////////////////////////////////
///                           FUCTIONS                               ///
////////////////////////////////////////////////////////////////////////

Force::Force(Chip& chip) : _chip(chip) {
    _row.reserve(_chip.getNumCells());
    _column.reserve(_chip.getNumCells());
    _rowNew.reserve(_chip.getNumCells());
    _columnNew.reserve(_chip.getNumCells());
    _rowSum.reserve(_chip.getNumNets());
    _columnSum.reserve(_chip.getNumNets());
    _movable.reserve(_chip.getMaxMove());

    for (unsigned i = 0; i < _chip.getNumCells(); ++i) {
        assert(_chip.getCell(i).getRow() < _chip.getNumRows());
        assert(_chip.getCell(i).getColumn() < _chip.getNumColumns());
        _row.push_back(_chip.getCell(i).getRow());
        // std::cout << "row[" << i << "]" << _row[i] << '\n';
        _column.push_back(_chip.getCell(i).getColumn());
        // std::cout << "column[" << i << "]" << _column[i] << '\n';
        _rowNew.push_back(0);
        _columnNew.push_back(0);
    }
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        _rowSum.push_back(0);
        _columnSum.push_back(0);
    }
}

void Force::balance(unsigned ite) {
    balance_init();
    balance_ite(ite - 1);
}

inline void Force::balance_init() {
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        NetSum(i);
    }
    for (unsigned i = 0; i < _chip.getNumCells(); ++i) {
        if (!_chip.getCell(i).movable(_chip.limited())) {
            _rowNew[i] = _row[i];
            _columnNew[i] = _column[i];
        } else {
            UpdateCell(i);
        }
    }
    argList list;
    for (unsigned i = 0; i < _chip.getNumCells(); ++i) {
        if (_chip.getCell(i).movable(_chip.limited())) {
            unsigned row = _row[i], column = _column[i],
                     rowNew = _rowNew[i], columnNew = _columnNew[i];
            unsigned rowDiff = row > rowNew ? row - rowNew : rowNew - row;
            unsigned columnDiff = column > columnNew ? column - columnNew : columnNew;
            list.push_back(std::make_pair(i, rowDiff + columnDiff));
        }
    }
    std::sort(list.begin(), list.end(), myfunc);
    for (auto i = list.begin(); i != list.end(); ++i) {
        if (i->second == 0) {
            break;
        }
        unsigned idx = i->first;
        _movable.push_back(idx);
        _column[idx] = _columnNew[idx];
        _row[idx] = _rowNew[idx];
    }
}

inline void Force::balance_ite(unsigned ite){
    for (unsigned i = 0; i < ite; ++i) {
        balance_in();
    }
}

inline void Force::balance_in() {
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        NetSum(i);
    }
    for (unsigned i = 0; i < _movable.size(); ++i) {
        UpdateCell(i);
    }
}

inline void Force::NetSum(unsigned idx) {
    unsigned rowSum = 0, columnSum = 0;
    GridNet& net = _chip.getNet(idx);
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        unsigned cell = _chip.getPin(net.getPinIdx(i)).get_cell_idx();
        rowSum += _row[cell];
        columnSum += _column[cell];
    }
    _rowSum[idx] = rowSum;
    _columnSum[idx] = columnSum;
    assert(rowSum < _chip.getNumRows()*net.getNumPin());
    assert(columnSum < _chip.getNumColumns()*net.getNumPin());
}

inline void Force::UpdateCell(unsigned idx) {
    Cell& cell = _chip.getCell(idx);
    double rowNew = 0, columnNew = 0;
    unsigned row = _row[idx], column = _column[idx];
    for (unsigned i = 0; i < cell.getNumPins(); ++i) {
        int net = _chip.getPin(cell.getPinIdx(i)).get_net_idx();
        if (net == -1) {
            continue;
        }
        assert(net >= 0);
        unsigned w = _chip.getNet(net).getNumPin() - 1;
        rowNew += double(_rowSum[net] - row)/w;
        columnNew += double(_columnSum[net] - column)/w;
    }
    unsigned newRow = unsigned(rowNew/cell.getNumPins() + 0.5);
    unsigned newColumn = unsigned(columnNew/cell.getNumPins() + 0.5);
    assert(newRow < _chip.getNumRows());
    assert(newColumn < _chip.getNumColumns());
    _rowNew[idx] = newRow;
    _columnNew[idx] = newColumn;
}

unsigned Force::HPWL() const {
    unsigned ret = 0;
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        ret += HPWL_NET(i);
    }
    return ret;
}

inline unsigned Force::HPWL_NET(unsigned idx) const {
    GridNet& net = _chip.getNet(idx);
    unsigned maxRow = 0, minRow = _chip.getNumRows(),
             maxColumn = 0, minColumn = _chip.getNumColumns();
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        unsigned cell = _chip.getPin(net.getPinIdx(i)).get_cell_idx();
        unsigned row = _row[cell],
                 column = _column[cell];
        assert(row < _chip.getNumRows());
        assert(column < _chip.getNumColumns());
        if (row > maxRow) {
            maxRow = row;
        }
        if (row < minRow) {
            minRow = row;
        }
        if (column > maxColumn) {
            maxColumn = column;
        }
        if (column < minColumn) {
            minColumn = column;
        }
    }
    assert(maxRow >= minRow && maxColumn >= minColumn);
    return maxRow - minRow + maxColumn - minColumn;
}

bool myfunc(std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b) {
    return a.second > b.second;
}