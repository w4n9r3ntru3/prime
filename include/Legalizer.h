/***********************************************************************

  FileName    [Legalizer.h]

  Author      [Kan-Hsuan, Lai]

  This file difines the Legalizer.

***********************************************************************/

#pragma once

#include "Chip.h"

class Legalizer
{
public:
    Legalizer(Chip& chip);

    bool checkChip(Chip& chip);

private:
    void _legalize(Chip& chip, int pos_x, int pos_y, Cell& cell);
    bool _isLegal(Chip& chip, int pos_x, int pos_y);
    int _minSupply(Chip& chip, int pos_x, int pos_y);
};