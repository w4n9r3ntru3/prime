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
    BoundingNet(Chip& chp, GridNet& net);
    BoundingNet(BoundingNet&& other);
    BoundingNet(const BoundingNet& other) = delete;

   private:
    unsigned _leftmost, _leftnext;
    unsigned _rightmost, _rightnext;
    unsigned _topmost, _topnext;
    unsigned _bottommost, _bottomnext;
    unsigned _leftmostpin;
    unsigned _rightmostpin;
    unsigned _topmostpin;
    unsigned _bottommostpin;

    // private function
    void updatePos(const unsigned row,
                   const unsigned column,
                   const unsigned pin);
};

class Place {
   public:
    Place(Chip& chp);

   private:
    Chip& _chp;
    safe::vector<BoundingNet> _nets;
};