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
    std::string buf;

    /*MaxCellMove <maxMoveCount>*/
    input >> str; // MaxCellMove
    assert(str == "MaxCellMove");
    input >> _maxMove; // <maxMoveCount>

    /*GGridBoundaryIdx <rowBeginIdx> <colBeginIdx> <rowEndIdx> <colEndIdx>*/
    input >> str; //GGridBoundaryIdx
    assert(str == "GGridBoundaryIdx");
    int rb,cb,re,ce;
    input >> rb >> cb >> re >> ce; //<rowBeginIdx> <colBeginIdx> <rowEndIdx> <colEndIdx>
    _rowBase = cb; _columnBase = rb;
    _rowRange = ce - cb; _columnRange = re - rb;

    /*NumLayers <LayerCount>
      Lay <LayerName> <Idx> <RoutingDirection> <defaultSupplyOfOneGGrid>*/
    input >> str; //NumLayers
    assert(str == "NumLayers");
    input >> _layer; //<LayerCount>
    _layers.reserve(_layer); for(int i = 0; i < _layer; ++i) { _layers.push_back(0); }
    ConstructCoordinate();
    for(int i = 0; i < _layer; ++i)
    {
        input >> str; //Lay
        assert(str == "Lay");
        input >> str; //<LayerName>
        int idx;
        input >> idx; //<Idx>
        _Layer2Idx[str] = idx;
        bool direction;
        input >> buf; //<RoutingDirection>
        if(buf == "H") direction = false;
        else if(buf == "V") direction = true;
        else assert(buf == "H" || buf == "V");
        int supply;
        input >> supply; //<defaultSupplyOfOneGGrid>
        Layer* l = new Layer(str,idx-1,direction,supply,_area);
        _layers[idx-1] = l;
    }
    //TODO

}

void PrimeMan::constructCoordinate()
{
    _area = _columnRange*_rowRange;
    _coordinates.reserve(_area);
    for(int i = 0; i < _columnRange; ++i)
    {
        for(int j = 0; j < _rowRange; ++j)
        {
            Coordinate* c = Coordinate(j,i,_layer);
            _coordinates.push_back(c);
        }
    }
}
