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

#include "../include/safe.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class MasterCellType {
   public:
    // Constructors
    MasterCellType(unsigned idx, unsigned layer);
    MasterCellType(const MasterCellType& mct) = delete;
    MasterCellType(MasterCellType&& mct);

    // Assignment
    MasterCellType& operator=(const MasterCellType& mct) = delete;
    MasterCellType& operator=(MasterCellType&& mct);

    // Modifier
    void AddBlkg(unsigned layer, int demand);
    void reservePin(size_t s);
    void AddPin(unsigned idx, unsigned layer);
    void AddExtraSame(unsigned MC, int demand, unsigned layer);
    void AddExtraadjH(unsigned MC, int demand, unsigned layer);

    // acceser
    unsigned getIdx() const;
    int getLayerDemand(unsigned layer) const;
    size_t getNumPins() const;
    unsigned getPinLayer(unsigned pin) const;
    safe::vector<unsigned>& getSameGridMC(unsigned layer);
    safe::vector<unsigned>& getadjHGridMC(unsigned layer);
    safe::vector<int>& getSameGridDemand(unsigned layer);
    safe::vector<int>& getadjHGridDemand(unsigned layer);

    // friend
    friend std::ostream& operator<<(std::ostream& os,
                                    const MasterCellType& MCT);

   private:
    unsigned _idx;
    safe::vector<unsigned> _pin2Layer;
    safe::vector<int> _LayerDemand;
    safe::vector<safe::vector<unsigned>> _SameGridMC;
    safe::vector<safe::vector<int>> _SameGridDemand;
    safe::vector<safe::vector<unsigned>> _adjHGridMC;
    safe::vector<safe::vector<int>> _adjHGridDemand;
};

std::ostream& operator<<(std::ostream& os, const MasterCellType& MCT); //to be updated
