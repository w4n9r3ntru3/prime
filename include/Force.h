/***********************************************************************

  FileName    [Force.h]

  Author      [Yang Chien Yi]

  This file defines the force-directed method.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Chip.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/*

 */

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

typedef safe::vector<std::pair<unsigned, unsigned>> argList; // arg, val

class Force {
   public:
    // constructor
    Force(Chip& chip);
    Force(const Force& f) = delete;
    Force(Force&& f) = delete;

    // assignment
    Force& operator=(const Force& f) = delete;
    Force& operator=(Force&& f) = delete;

    // calculation
    void balance(unsigned ite); //number of iterations

    // estimation
    unsigned HPWL() const;
    unsigned HPWL_NET(unsigned idx) const;

   private:
    Chip& _chip;
    safe::vector<unsigned> _row;
    safe::vector<unsigned> _column;
    safe::vector<unsigned> _rowSum; // by net
    safe::vector<unsigned> _columnSum; // by net
    safe::vector<unsigned> _rowNew;
    safe::vector<unsigned> _columnNew;
    safe::vector<unsigned> _movable; // movable cells

    // private function
    void balance_init();
    void balance_ite(unsigned ite);
    void balance_in();
    void NetSum(unsigned idx);
    void UpdateCell(unsigned idx);
};

bool myfunc(std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b);