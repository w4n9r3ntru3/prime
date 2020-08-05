/***********************************************************************

  FileName    [place.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file defines the placement method considering only the pins on
  the bounding box.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Chip.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class BoundingNet {
   public:
    BoundingNet(GridNet& net);

   private:
    unsigned leftmax, leftnext;
    unsigned rightmax, rightnext;
    unsigned topmax, topnext;
    unsigned bottommax, bottomnext;
    unsigned leftmaxpin;
    unsigned rightmaxpin;
    unsigned topmaxpin;
    unsigned bottommaxpin;
};

class Place {
   public:
    Place(Chip& chp);

   private:
    Chip& _chp;
    safe::vector<BoundingNet> _nets;
};