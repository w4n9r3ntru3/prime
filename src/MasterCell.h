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

#include <iostream>
#include <string>

#include "safe.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class MasterCellType;

class PinType {
   public:
    // Constructors
    PinType(const std::string PinName, int layer, MasterCellType& MCT);
    // PinType(const PinType& a);
    PinType(PinType&& a);

    // PinType& operator=(const PinType& a);
    PinType& operator=(PinType&& a);

    // acceser
    const std::string& getPinName() const;
    int getLayer() const;

    // friend
    friend std::ostream& operator<<(std::ostream& os, const PinType& PT);

   private:
    std::string _PinName;
    int _layer;
    MasterCellType& _MCT;
};

std::ostream& operator<<(std::ostream& os, const PinType& PT);

class BlockageType {
   public:
    // Constructors
    BlockageType(const std::string BlkgName, int layer, int demand);
    BlockageType(const BlockageType& a);
    BlockageType(BlockageType&& a);

    BlockageType& operator=(const BlockageType& a);
    BlockageType& operator=(BlockageType&& a);

    // accesor
    const std::string& getBlkgName() const;
    int getLayer() const;
    int getDemand() const;

    // friend
    friend std::ostream& operator<<(std::ostream& os, const BlockageType& BT);

   private:
    std::string _BlkgName;
    int _layer;
    int _demand;
};

std::ostream& operator<<(std::ostream& os, const BlockageType& BT);

class MasterCellType {
   public:
    // Constructor(no copy constructor)
    MasterCellType(const std::string MCName, unsigned id, int layer);
    MasterCellType(MasterCellType&& mct);

    // ! no need for destructor
    // ~MasterCellType();

    MasterCellType& operator=(MasterCellType&& a);

    // Modifier
    void AddBlkg(const std::string BlkgName, int layer, int demand);
    void AddPin(const std::string PinName, int layer);
    void AddExtraSame(unsigned MC, int demand, int layer);
    void AddExtraadjH(unsigned MC, int demand, int layer);

    // acceser
    const std::string& getMCName() const;
    unsigned getId() const;
    unsigned getNumLayers() const;
    int getLayerDemand(int i) const;
    size_t getNumPins() const;
    size_t getNumBlkgs() const;
    PinType& getPinType(size_t i);
    PinType& getPinType(std::string& str);
    size_t getPin(std::string& str) const;
    BlockageType& getBlkg(size_t i);
    safe::vector<unsigned>& getSameGridMC(unsigned layer);
    safe::vector<unsigned>& getadjHGridMC(unsigned layer);
    safe::vector<int>& getSameGridDemand(unsigned layer);
    safe::vector<int>& getadjHGridDemand(unsigned layer);

    // friend
    friend std::ostream& operator<<(std::ostream& os,
                                    const MasterCellType& MCT);

   private:
    std::string _MCName;
    unsigned _Id;
    int _layer;

    safe::vector<int> _LayerDemand;
    safe::vector<PinType> _Pins;
    safe::vector<BlockageType> _Blkgs;
    safe::vector<safe::vector<unsigned>> _SameGridMC;
    safe::vector<safe::vector<int>> _SameGridDemand;
    safe::vector<safe::vector<unsigned>> _adjHGridMC;
    safe::vector<safe::vector<int>> _adjHGridDemand;
    safe::unordered_map<std::string, unsigned> _PinName2Idx;
};

std::ostream& operator<<(std::ostream& os, const MasterCellType& MCT);
