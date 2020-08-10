/***********************************************************************

  FileName    [Legalizer.cpp]

  Author      [Kan-Hsuan, Lai]

  This file describes the functions in "Legalizer.h".

***********************************************************************/

#include "Legalizer.h"
#include "Grid.h"

Legalizer::Legalizer(Chip& chip)
{
    
    for(unsigned i = 0; i < chip.getNumCells(); i++)
    {
        Cell& cell = chip.getCell(i);
        int x = cell.getRow(), y = cell.getColumn();
        if(!_isLegal(chip, x, y) && cell.movable(chip.limited()))
        {
            _legalize(chip, x, y, cell);
        }
    }

    assert(checkChip(chip));
}

bool Legalizer::checkChip(Chip& chip)
{
    for(unsigned x = chip.getRowBase(); x < chip.getNumRows(); x++)
    {
        for(unsigned y = chip.getColumnBase(); y < chip.getNumColumns(); y++)
        {
            if(!_isLegal(chip, x, y)) return false;
        }
    }
    return true;
}

bool Legalizer::_isLegal(Chip& chip, int pos_x, int pos_y)
{
    return _minSupply(chip, pos_x, pos_y) < 2;
}

int Legalizer::_minSupply(Chip& chip, int pos_x, int pos_y)
{
    int min = 10000000;
    for(unsigned z = 0; z < chip.getNumLayers(); z++)
    {
        int m = chip.getLayer(z).getGrid(chip.getIdx(pos_x, pos_y)).getSupply();
        if(m < min)
        {
            min = m;
        }
    }
    return min;
}

void Legalizer::_legalize(Chip& chip, int pos_x, int pos_y, Cell& cell)
{
    for(int n = 0; n < 5; n++)
    {
        int max = -1;
        int dest_x = -1, dest_y = -1;

        for(int x = 0; x <= n; x++)
        {
            int y = n - x;

            int dx = pos_x + x;
            int dy = pos_y + y;
            if(chip.isValidPosition(0, dx, dy) && _isLegal(chip, dx, dy))
            {
                chip.moveCell(cell, pos_x, pos_y, dx, dy);
                int ms = _minSupply(chip, dx, dy);
                if(ms > max)
                {
                    max = ms;
                    dest_x = dx;
                    dest_y = dy;
                }
                // chip.revert(cell, pos_x, pos_y, dx, dy);
            }

            dx = pos_x - x;
            dy = pos_y + y;
            if(chip.isValidPosition(0, dx, dy) && _isLegal(chip, dx, dy))
            {
                chip.moveCell(cell, pos_x, pos_y, dx, dy);
                int ms = _minSupply(chip, dx, dy);
                if(ms > max)
                {
                    max = ms;
                    dest_x = dx;
                    dest_y = dy;
                }
                // chip.revert(cell, pos_x, pos_y, dx, dy);
            }

            dx = pos_x + x;
            dy = pos_y - y;
            if(chip.isValidPosition(0, dx, dy) && _isLegal(chip, dx, dy))
            {
                chip.moveCell(cell, pos_x, pos_y, dx, dy);
                int ms = _minSupply(chip, dx, dy);
                if(ms > max)
                {
                    max = ms;
                    dest_x = dx;
                    dest_y = dy;
                }
                // chip.revert(cell, pos_x, pos_y, dx, dy);
            }

            dx = pos_x - x;
            dy = pos_y - y;
            if(chip.isValidPosition(0, dx, dy) && _isLegal(chip, dx, dy))
            {
                chip.moveCell(cell, pos_x, pos_y, dx, dy);
                int ms = _minSupply(chip, dx, dy);
                if(ms > max)
                {
                    max = ms;
                    dest_x = dx;
                    dest_y = dy;
                }
                // chip.revert(cell, pos_x, pos_y, dx, dy);
            }
        }

        if(max != -1)
        {
            chip.moveCellLegal(cell, pos_x, pos_y, dest_x, dest_y);
        }
    }
}