/***********************************************************************

  FileName    [PrimeMan.cpp]

  Author      [Yang Chien Yi]

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
    constructCoordinate();
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
        assert(_MasterCell2Idx.count(str) == 0);
        _MasterCell2Idx[str] = i;
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
    int mc1, mc2;
    input >> str; //NumNeighborCellExtraDemand
    assert(str == "NumNeighborCellExtraDemand");
    input >> count; //<count>
    for(int i = 0; i < count; ++i)
    {
        input >> str; //sameGGrid || adjHGGrid
        input >> buf; // <masterCellName1>
        mc1 = _MasterCell2Idx[buf];
        input >> buf; // <masterCellName2>
        mc2 = _MasterCell2Idx[buf];
        input >> buf >> demand; // <layerName> <demand>
        layer = _Layer2Idx[buf];
        if(str == "sameGGrid") { _MasterCells[mc1]->AddExtraSame(mc2,demand,layer); _MasterCells[mc2]-> AddExtraSame(mc1,demand,layer);}
        else if(str == "adjHGGrid") { _MasterCells[mc1]->AddExtraadjH(mc2,demand,layer); _MasterCells[mc2]-> AddExtraadjH(mc1,demand,layer);}
        else assert(str == "sameGGrid" || str == "adjHGGrid");
    }

    /*NumCellInst <cellInstCount>
      CellInst <instName> <masterCellName> <gGridRowIdx> <gGridColIdx> <movableCstr>*/
    input >> str; // NumCellInst
    assert(str == "NumCellInst");
    input >> count; // <cellInstCount>
    _cells.reserve(count);
    for(int i = 0; i < count; ++i)
    {
        input >> str; // CellInst
        assert(str == "CellInst");
        input >> str; // <instName>
        assert(_Cell2Idx.count(str) == 0);
        _Cell2Idx[str] = i;
        input >> buf; // <masterCellName>
        assert(_MasterCell2Idx[buf] == 1);
        MasterCellType MCT = *_MasterCells[_MasterCell2Idx[buf]];
        input >> row >> column >> buf; // <gGridRowIdx> <gGridColIdx> <movableCstr>
        bool movable;
        if(buf == "Movable") movable = true;
        else if(buf == "Fixed") movable = false;
        else assert(buf == "Fixed" || buf == "Movable");
        Cell* cell = new Cell(str,MCT,movable,i);
        _cells.push_back(cell);
        int rIdx = row - _rowBase, cIdx = column - _columnBase;
        cell->setCoordinate(rIdx,cIdx);
        Coordinate* c = _coordinates[getIdx(rIdx,cIdx)];
        int left, right;
        left = getLeft(rIdx,cIdx); right = getRight(rIdx,cIdx);
        Coordinate* c1 = 0;
        Coordinate* c2 = 0;
        if(left > -1) c1 = _coordinates[left];
        if(right > -1) c2 = _coordinates[right];
        c->addCell(cell,c1,c2);
    }

    /*NumNets <netCount>
      Net <netName> <numPins> <minRoutingLayConstraint>
      Pin <instName>/<masterPinName>*/
    input >> str; // NumNets
    assert(str == "NumNets");
    input >> count; // <netCount>
    for(int i = 0; i < count; ++i)
    {
        int numPins;
        input >> str; // Net
        assert(str == "Net");
        input >> str; // <netName>
        input >> numPins; // <numPins>
        assert(_Net2Idx.count(str) == 0);
        _Net2Idx[str] = i;
        Net* net = new Net(str,i,numPins,_layer);
        _nets.push_back(net);
        std::string inst, masterPin;
        std::string delimiter = "/";
        for(int j = 0; j < numPins; ++j)
        {
            input >> str; // <instName>/<masterPinName>
            size_t pos = str.find(delimiter);
            inst = str.substr(0,pos);
            pos++;
            masterPin = str.substr(pos,str.size()-pos);
            assert(_Cell2Idx.count(inst) == 1);
            Cell* cell = _cells[_Cell2Idx[inst]];
            Pin pin = cell->getPin(inst);
            net->addPin(&pin);
        }
    }

    /*NumRoutes <routeSegmentCount>
      <sRowIdx> <sColIdx> <sLayIdx> <eRowIdx> <eColIdx> <eLayIdx> <netName>*/
    int srow, scol, slay, erow, ecol, elay;
    input >> str; // NumRoutes
    assert(str == "NumRoutes");
    input >> count; // <routeSegmentCount>
    for(int i = 0; i < count; ++i)
    {
        input >> srow >> scol >> slay >> erow >> ecol >> elay >> str; // <sRowIdx> <sColIdx> <sLayIdx> <eRowIdx> <eColIdx> <eLayIdx> <netName>

    }
}

PrimeMan::~PrimeMan()
{
    for(int i = 0, n = _layers.size(); i < n; ++i) delete _layers[i];
    for(int i = 0, n = _coordinates.size(); i < n; ++i) delete _coordinates[i];
    for(int i = 0, n = _MasterCells.size(); i < n; ++i) delete _MasterCells[i];
    for(int i = 0, n = _cells.size(); i < n; ++i) delete _cells[i];
}

void PrimeMan::constructCoordinate()
{
    _area = _columnRange*_rowRange;
    _coordinates.reserve(_area);
    for(int i = 0; i < _columnRange; ++i)
    {
        for(int j = 0; j < _rowRange; ++j)
        {
            Coordinate* c = new Coordinate(j,i,_layer);
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
            g.assignCoordinate(c);
            c->addGrid(&g);
        }
    }
}