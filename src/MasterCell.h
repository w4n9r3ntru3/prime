/***********************************************************************

  FileName    [MasterCell.h]

  Author      [Yang, Chien Yi]

  This file defines the master cells and their elements, pins and
  blockages.

***********************************************************************/

#ifndef MASTERCELL_H
#define MASTERCELL_H

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <unordered_map>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class MasterCellType;

class PinType
{
public:
    //Constructors
    PinType(const std::string&  PinName, int layer, MasterCellType& MCT):
    _PinName(PinName), _layer(layer), _MCT(MCT)
    { assert(layer >= 0); }

    PinType(PinType& a) :
    _PinName(a._PinName), _layer(a._layer), _MCT(a._MCT)
    { assert(a._layer >= 0); }

    //acceser
    std::string getPinName() const                                  { return _PinName; }
    int         getLayer() const                                    { return _layer; }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const PinType& PT);

private:
    const std::string                   _PinName;
    const int                           _layer;
    const MasterCellType                _MCT;
};

std::ostream& operator<<(std::ostream& os, const PinType& PT)
{ os << "Pin Name : " << PT._PinName << " Layer : " << PT._layer << '\n'; return os; }

class BlockageType
{
public:
    //Constructors
    BlockageType(const std::string& BlkgName, int layer, int demand):
    _BlkgName(BlkgName), _layer(layer), _demand(demand)
    { assert(layer>=0 && demand>=0); }

    BlockageType(BlockageType& a):
    _BlkgName(a._BlkgName), _layer(a._layer), _demand(a._demand)
    { assert(a._layer>=0 && a._demand>=0); }

    //acceser
    std::string getBlkgName() const                                 { return _BlkgName; }
    int         getLayer() const                                    { return _layer; }
    int         getDemand() const                                   { return _demand; }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const BlockageType& BT);

private:
    const std::string                   _BlkgName;
    const int                           _layer;
    const int                           _demand;
};

std::ostream& operator<<(std::ostream& os, const BlockageType& BT)
{ os << "Blockage Name : " << BT._PinName << " Layer : " << BT._layer << " Demand : " << BT._demand << '\n'; return os; }

class MasterCellType
{
public:
    //Constructor(no copy constructor)
    MasterCellType(const std::string& MCName, unsigned id, int layer): _MCName(MCName), _Id(id)
    {
        _LayerDemand.reserve(layer); for (int i = 0;i < layer; ++i) _LayerDemand.push_back(0);
    }

    //Modifier
    void AddBlkg(const std::string& BlkgName, int layer, int demand)
    {
        _Blkgs.push_back(BlockageType(BlkgName, layer, demand));
        _LayerDemand[layer] += demand;
    }
    void MasterCellType::AddPin(const std::string& PinName, int layer)
    {
        _PinName2Idx[PinName] = _Pins.size();
        _Pins.push_back(PinType(PinName, layer, *this));
    }
    void AddExtraSame(unsigned MC, int demand)
    {
        _ExtraSameGrid[MC] = _SameGridDemand.size();
        _SameGridDemand.push_back(demand);
    }
    void AddExtraadjH(unsigned MC, int demand)
    {
        _ExtraadjHGrid[MC] = _HGridDemand.size();
        _adjHGridDemand.push_back(demand);
    }

    //acceser
    std::string getMCName() const                                   { return _MCName; }
    unsigned getId() const                                          { return _Id; }
    int getLayerDemand(int i) const                                 { assert(i < _LayerDemand.size()); return _LayerDemand[i]; }
    size_t getNumPins() const                                       { return _Pins.size(); }
    size_t getNumBlkgs() const                                      { return _Blkgs.size(); }
    PinType& getPinType(size_t i) const                             { assert(i < _Pins.size()); return _Pins[i]; }
    PinType& getPinType(std::string& str) const                     { auto idx = _PinName2Idx.find(str); assert(idx != _PinName2Idx.end()); return _Pins[idx->second]; }
    size_t getPin(std::string& str) const                           { auto idx = _PinName2Idx.find(str); assert(idx != _PinName2Idx.end()); return idx->second; }
    BlockageType& getBlkg(size_t i) const                           { assert(i < _Blkgs.size()); return _Blkgs[i]; }
    int getDemandSame(unsigned a) const                             
    {
        auto idx = _ExtraSameGrid.find(a);
        if(idx != _ExtraSameGrid.end()) return _SameGridDemand[idx->second];
        return 0;
    }
    int getDemandSame(const MasterCellType& a) const
    {
        auto idx = _ExtraSameGrid.find(a._Id);
        if(idx != _ExtraSameGrid.end()) return _SameGridDemand[idx->second];
        return 0;
    }
    int getDemandadjH(unsigned a) const                             
    {
        auto idx = _ExtraadjHGrid.find(a);
        if(idx != _ExtraadjHGrid.end()) return _adjHGridDemand[idx->second];
        return 0;
    }
    int getDemandadjH(const MasterCellType& a) const
    {
        auto idx = _ExtraadjHGrid.find(a._Id);
        if(idx != _ExtraadjHGrid.end()) return _adjHGridDemand[idx->second];
        return 0;
    }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const MasterCellType& MCT);

private:
    const std::string                           _MCName;
    const unsigned                              _Id;
    std::vector<int>                            _LayerDemand;
    std::vector<PinType>                        _Pins;
    std::vector<BlockageType>                   _Blkgs;
    std::unordered_map<unsigned,unsigned>       _ExtraSameGrid;
    std::vector<int>                            _SameGridDemand;
    std::unordered_map<unsigned,unsigned>       _ExtraadjHGrid;
    std::vector<int>                            _adjHGridDemand;
    std::unordered_map<std::string,unsigned>    _PinName2Idx;
};

std::ostream& operator<<(std::ostream& os, const MasterCellType& MCT)
{
    os << "Master Cell Name : " << MCT._MCName << " Id : " << MCT._Id << '\n'
       << "Pins : \n";
    for(size_t i = 0, n = MCT._Pins.size(); i < n; ++i) os << MCT._Pins[i];
    os << "Blockages : \n";
    for(size_t i = 0, n = MCT._Blkgs.size(); i < n; ++i) os << MCT._Blkgs[i];
    os << "Same Grid Extra Demand : \n";
    for(size_t i = 0, n = _SameGridDemand.size(); i < n; ++i) os << i << " : " << _SameGridDemand[i] << '\n';
    os << "Same Grid Extra Demand : \n";
    for(size_t i = 0, n = _adjHGridDemand.size(); i < n; ++i) os << i << " : " << _adjHGridDemand[i] << '\n';
    os << "---------------------------------------------" << '\n';
    return os;
}

#endif