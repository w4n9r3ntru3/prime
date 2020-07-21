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
#define StepSize 1000
#define gamma 300
// #define lambda_ 3.
#define WIRELENTH_WA

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

typedef safe::vector<std::pair<unsigned, double>> argList; // arg, val

class Cost {
   public:
    Cost(Chip& chip);
    ~Cost() {}

    void evaluateFG(const safe::vector<double>& x,
                    double& f,
                    safe::vector<double>& g);
    void evaluateF(const safe::vector<double>& x, double& f);

    unsigned HPWL(const safe::vector<double>& x) const;
    inline unsigned HPWL_NET(unsigned idx, const safe::vector<double>& x) const;

   private:
    Chip& _chip;
    safe::vector<double> _f_max_val, _f_min_val, _f_max_wei,
        _f_min_wei;  // buffer for calculating f; odd for y, even for x
    safe::unordered_set<unsigned> _movable;
    double Wirelength(const safe::vector<double>& x);
    double Density(const safe::vector<double>& x);
};

bool myfunc(std::pair<unsigned, double> a, std::pair<unsigned, double> b);