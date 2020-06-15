// FIXME
// this class is diabled because of missing parts

#ifdef FORCE

/***********************************************************************

  FileName    [Force.h]

  Author      [Yang Chien Yi]

  This file defines the force directed method.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "BucketList.h"
#include "PrimeMan.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Force {
   public:
    // Constructor
    Force(PrimeMan& pm);

    // solver
    void solve();

   private:
    PrimeMan& _pm;
    BucketList* _bkList;
    std::vector<Node*> _nodes;
};

#endif
