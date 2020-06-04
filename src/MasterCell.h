/***********************************************************************

  FileName    [MasterCell.h]

  Author      [Yang Chien Yi]

  This file defines the master cells and their elements, pins and
  blockages.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <assert.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "safe.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class MasterCellType;

class PinType {
   public:
    // Constructors
    PinType(const std::string PinName, int layer, MasterCellType& MCT)
        : _PinName(PinName), _layer(layer), _MCT(MCT) {
        assert(layer >= 0);
    }

    PinType(PinType& a) : _PinName(a._PinName), _layer(a._layer), _MCT(a._MCT) {
        assert(a._layer >= 0);
    }

    // acceser
    const std::string& getPinName() const { return _PinName; }
    int getLayer() const { return _layer; }

    // friend
    friend std::ostream& operator<<(std::ostream& os, const PinType& PT);

   private:
    const std::string _PinName;
    const int _layer;
    const MasterCellType& _MCT;
};

std::ostream& operator<<(std::ostream& os, const PinType& PT) {
    os << "Pin Name : " << PT._PinName << " Layer : " << PT._layer << '\n';
    return os;
}

class BlockageType {
   public:
    // Constructors
    BlockageType(const std::string BlkgName, int layer, int demand)
        : _BlkgName(BlkgName), _layer(layer), _demand(demand) {
        assert(layer >= 0 && demand >= 0);
    }

    BlockageType(BlockageType& a)
        : _BlkgName(a._BlkgName), _layer(a._layer), _demand(a._demand) {
        assert(a._layer >= 0 && a._demand >= 0);
    }

    // acceser
    std::string getBlkgName() const { return _BlkgName; }
    int getLayer() const { return _layer; }
    int getDemand() const { return _demand; }

    // friend
    friend std::ostream& operator<<(std::ostream& os, const BlockageType& BT);

   private:
    const std::string _BlkgName;
    const int _layer;
    const int _demand;
};

std::ostream& operator<<(std::ostream& os, const BlockageType& BT) {
    os << "Blockage Name : " << BT._BlkgName << " Layer : " << BT._layer
       << " Demand : " << BT._demand << '\n';
    return os;
}

class MasterCellType {
   public:
    // Constructor(no copy constructor)
    MasterCellType(const std::string MCName, unsigned id, int layer)
        : _MCName(MCName), _Id(id), _layer(layer) {
        _LayerDemand.reserve(layer);
        _SameGridMC.reserve(layer);
        _adjHGridMC.reserve(layer);
        _SameGridDemand.reserve(layer);
        _adjHGridDemand.reserve(layer);
        for (int i = 0; i < layer; ++i) {
            _LayerDemand.push_back(0);
            safe::vector<unsigned>* v1 = new safe::vector<unsigned>();
            _SameGridMC.push_back(v1);
            safe::vector<unsigned>* v2 = new safe::vector<unsigned>();
            _adjHGridMC.push_back(v2);
            safe::vector<int>* v3 = new safe::vector<int>();
            _SameGridDemand.push_back(v3);
            safe::vector<int>* v4 = new safe::vector<int>();
            _adjHGridDemand.push_back(v4);
        }
    }

    // destructor
    ~MasterCellType() {
        for (int i = 0; i < _layer; ++i) {
            delete _adjHGridDemand[i];
            delete _adjHGridMC[i];
            delete _SameGridDemand[i];
            delete _SameGridMC[i];
        }
    }

    // Modifier
    void AddBlkg(const std::string BlkgName, int layer, int demand) {
        _Blkgs.push_back(BlockageType(BlkgName, layer, demand));
        _LayerDemand[layer] += demand;
    }
    void AddPin(const std::string PinName, int layer) {
        _PinName2Idx[PinName] = _Pins.size();
        _Pins.push_back(PinType(PinName, layer, *this));
    }
    void AddExtraSame(unsigned MC, int demand, int layer) {
        _SameGridMC[layer]->push_back(MC);
        _SameGridDemand[layer]->push_back(demand);
    }
    void AddExtraadjH(unsigned MC, int demand, int layer) {
        _adjHGridMC[layer]->push_back(MC);
        _SameGridDemand[layer]->push_back(demand);
    }

    // acceser
    const std::string& getMCName() const { return _MCName; }
    unsigned getId() const { return _Id; }
    unsigned getNumLayers() const { return _layer; }
    int getLayerDemand(int i) const {
        assert(i < int(_LayerDemand.size()));
        return _LayerDemand[i];
    }
    size_t getNumPins() const { return _Pins.size(); }
    size_t getNumBlkgs() const { return _Blkgs.size(); }
    PinType& getPinType(size_t i) {
        assert(i < _Pins.size());
        return _Pins[i];
    }
    PinType& getPinType(std::string& str) {
        auto idx = _PinName2Idx.find(str);
        assert(idx != _PinName2Idx.end());
        return _Pins[idx->second];
    }
    size_t getPin(std::string& str) const {
        auto idx = _PinName2Idx.find(str);
        assert(idx != _PinName2Idx.end());
        return idx->second;
    }
    BlockageType& getBlkg(size_t i) {
        assert(i < _Blkgs.size());
        return _Blkgs[i];
    }
    safe::vector<unsigned>& getSameGridMC(unsigned layer) {
        assert(layer < _SameGridMC.size());
        return *_SameGridMC[layer];
    }
    safe::vector<unsigned>& getadjHGridMC(unsigned layer) {
        assert(layer < _adjHGridMC.size());
        return *_adjHGridMC[layer];
    }
    safe::vector<int>& getSameGridDemand(unsigned layer) {
        assert(layer < _SameGridDemand.size());
        return *_SameGridDemand[layer];
    }
    safe::vector<int>& getadjHGridDemand(unsigned layer) {
        assert(layer < _adjHGridDemand.size());
        return *_adjHGridDemand[layer];
    }

    // friend
    friend std::ostream& operator<<(std::ostream& os,
                                    const MasterCellType& MCT);

   private:
    const std::string _MCName;
    const unsigned _Id;
    const int _layer;
    safe::vector<int> _LayerDemand;
    safe::vector<PinType> _Pins;
    safe::vector<BlockageType> _Blkgs;
    safe::vector<safe::vector<unsigned>*> _SameGridMC;
    safe::vector<safe::vector<int>*> _SameGridDemand;
    safe::vector<safe::vector<unsigned>*> _adjHGridMC;
    safe::vector<safe::vector<int>*> _adjHGridDemand;
    std::unordered_map<std::string, unsigned> _PinName2Idx;
};

std::ostream& operator<<(std::ostream& os, const MasterCellType& MCT) {
    os << "Master Cell Name : " << MCT._MCName << " Id : " << MCT._Id << '\n'
       << "Pins : \n";
    for (size_t i = 0, n = MCT._Pins.size(); i < n; ++i) {
        os << MCT._Pins[i];
    }
    os << "Blockages : \n";
    for (size_t i = 0, n = MCT._Blkgs.size(); i < n; ++i) {
        os << MCT._Blkgs[i];
    }
    os << "---------------------------------------------" << '\n';
    return os;
}
