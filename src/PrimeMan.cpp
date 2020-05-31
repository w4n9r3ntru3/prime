/***********************************************************************

  FileName    [PrimeMan.cpp]

  Author      [Yang, Chien Yi]

  This file describes the functions in "PrimeMan.h".

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "PrimeMan.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

PrimeMan::PrimeMan(std::fstream& input)
{
    readFile(input);
}

void PrimeMan::readFile(std::fstream& input)
{
    std::string str;

    /*MaxCellMove <maxMoveCount>*/
    input >> str;
    assert(str == "MaxCellMove");
    input >> _maxMove;

    /*GGridBoundaryIdx <rowBeginIdx> <colBeginIdx> <rowEndIdx> <colEndIdx>*/
    input >> str;
    assert(str == "GGridBoundaryIdx");
    int rb,cb,re,ce;
    input >> rb >> cb >> re >> ce;

    /*NumLayers <LayerCount>
      Lay <LayerName> <Idx> <RoutingDirection> <defaultSupplyOfOneGGrid>*/
    input >> str;
    assert(str == "NumLayers");
    int layer;
    input >> layer;
    for (int i = 0; i < layer; ++i)
    {
        
    }
}