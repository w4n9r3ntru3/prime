#ifdef FORCE

/***********************************************************************

  FileName    [Force.cpp]

  Author      [Yang Chien Yi]

  This file describes the force directed method.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Force.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

Force::Force(PrimeMan& pm) : _pm(pm) {
    _bkList = new BucketList();
    for (int i = 0, n = _pm.getNumCells(); i < n; ++i) {
        Node* node = new Node(i);
        _nodes.push_back(node);
    }
    int maxPin = 0;
    for (int i = 0, n = _pm.getNumMasterCells(); i < n; ++i) {
        MasterCellType& mct = _pm.getMasterCell(i);
        int numPin = mct.getNumPins();
        if (numPin > maxPin) maxPin = numPin;
    }
    _bkList->init(maxPin);
}

void Force::solve() { 
  
}

#endif
