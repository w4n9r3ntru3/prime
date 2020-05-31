/***********************************************************************

  FileName    [PrimeMan.h]

  Author      [Yang, Chien Yi]

  This file difines the data manager for prime.

***********************************************************************/

#ifndef PRIMEMAN_H
#define PRIMEMAN_H

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include<fstream>

#include "Cell.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class PrimeMan
{
public:
    //Constructor
    PrimeMan(std::fstream& input);
private:
    unsigned                  _maxMove;

    //private function
    void readFile(std::fstream& input);
    void constructChip();
};


#endif