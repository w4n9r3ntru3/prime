/***********************************************************************

  FileName    [Chip.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file difines the data manager for prime.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <assert.h>

#include <cmath>

#include "Chip.h"

//#include "QuadTree.h"
// #include "TreeNet.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

// #define iter 100
#define StepSize    1000
#define gamma       300
#define lambda_ 3.
#define WIRELENTH_WA


////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Cost {
   public:
    Cost(Chip& chip);
    ~Cost() {}

    void evaluateFG(const safe::vector<double>& x, double& f, safe::vector<double>& g);
    void evaluateF(const safe::vector<double>& x, double& f);
    unsigned dimension();

   private:
    Chip& _chip;
    safe::vector<double> _f_max_val, _f_min_val, _f_max_wei,
        _f_min_wei;  // buffer for calculating f; odd for y, even for x
    double Wirelength(const safe::vector<double>& x);
    double Density(const safe::vector<double>& x);

};