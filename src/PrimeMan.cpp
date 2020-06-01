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
    connectCoordinateGrid();
    
    /*NumNonDefaultSupplyGGrid <nonDefaultSupplyGGridCount>
      <rowIdx> <colIdx> <LayIdx> <incrOrDecrValue>*/
    input >> str; //NumNonDefaultSupplyGGrid
    assert(str == "NumNonDefaultSupplyGGrid");
    int count, row, column, layer, val;
    input >> count;
    for(int i = 0; i < count; ++i)
    {
        input >> row >> column >> layer >> val;
        _layers[row-1]->getGrid(getIdx(row,column)).incSupply(val);
    }
    
    /*NumMasterCell <masterCellCount>
      MasterCell <masterCellName> <pinCount> <blockageCount>
      Pin <pinName> <pinLayer>
      Blkg <blockageName> <blockageLayer> <demand>
    */
    int demand;
    input >> str; // NumMasterCell
    assert(str == "NumMasterCell");
    input >> count; //<masterCellCount>
    _MasterCells.reserve(count);
    for(int i = 0; i < count; ++i)
    {
        input >> str; // MasterCell
        assert(str == "MasterCell");
        input >> str; // <masterCellName>
        MasterCellType* mct = new MasterCellType(str,i,_layer);
        _MasterCells.push_back(mct);
        int pin, blockage;
        input >> pin >> blockage;
        for(int j = 0; j < pin; ++j)
        {
            input >> str; // Pin
            assert(str == "Pin");
            input >> str >> layer; // <pinName> <pinLayer>
            mct->AddPin(str,layer);
        }
        for(int j = 0; j < blockage; ++j)
        {
            input >> str; // Blkg
            assert(str == "Blkg");
            input >> str >> layer >> demand; // <blockageName> <blockageLayer> <demand>
            mct->AddBlkg(str,layer,demand);
        }
    }

    /*NumNeighborCellExtraDemand <count>
      sameGGrid <masterCellName1> <masterCellName2> <layerName> <demand>
      adjHGGrid <masterCellName1> <masterCellName2> <layerName> <demand>*/
    input >> str; //NumNeighborCellExtraDemand
    assert(str == "NumNeighborCellExtraDemand");
    input >> count; //<count>
    for(int i = 0; i < count; ++i)
    {
        input >> str; //sameGGrid || adjHGGrid
        if(str == "sameGGrid")
        {

        }
        else if(str == "adjHGGrid") 
        else assert(str == "sameGGrid" || str == "adjHGGrid");
    }
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

void PrimeMan::connectCoordinateGrid()
{
    for(int i = 0;i < _area; ++i)
    {
        Coordinate* c = _coordinates[i];
        for(int j = 0; j < _layer; ++j)
        {
            Layer* l = _layers[j];
            Grid g = l->getGrid(i);
            g.assignCoordinate(*c);
            c->addGrid(&g);
        }
    }
}