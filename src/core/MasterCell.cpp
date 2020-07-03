/***********************************************************************

  FileName    [MasterCell.cpp]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file describes the master cells and their elements, pins and
  blockages.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "../include/MasterCell.h"

#include <assert.h>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

// MasterCellType
MasterCellType::MasterCellType(unsigned idx, unsigned layer)
    : _idx(idx)
{
    _LayerDemand.reserve(layer);
    _SameGridMC.reserve(layer);
    _adjHGridMC.reserve(layer);
    _SameGridDemand.reserve(layer);
    _adjHGridDemand.reserve(layer);
    for (unsigned i = 0; i < layer; ++i)
    {
        _LayerDemand.push_back(0);
        safe::vector<unsigned> v1;
        _SameGridMC.push_back(std::move(v1));
        safe::vector<unsigned> v2;
        _adjHGridMC.push_back(std::move(v2));
        safe::vector<int> v3;
        _SameGridDemand.push_back(std::move(v3));
        safe::vector<int> v4;
        _adjHGridDemand.push_back(std::move(v4));
    }
}

// MasterCellType::MasterCellType(const MasterCellType &mct)
//     : _idx(mct._idx),
//       _pin2Layer(mct._pin2Layer),
//       _LayerDemand(mct._LayerDemand),
//       _SameGridMC(mct._SameGridMC),
//       _SameGridDemand(mct._SameGridDemand),
//       _adjHGridMC(mct._adjHGridMC),
//       _adjHGridDemand(mct._adjHGridDemand) {}

MasterCellType::MasterCellType(MasterCellType &&mct)
    : _idx(mct._idx),
      _pin2Layer(std::move(mct._pin2Layer)),
      _LayerDemand(std::move(mct._LayerDemand)),
      _SameGridMC(std::move(mct._SameGridMC)),
      _SameGridDemand(std::move(mct._SameGridDemand)),
      _adjHGridMC(std::move(mct._adjHGridMC)),
      _adjHGridDemand(std::move(mct._adjHGridDemand)) {}

MasterCellType& MasterCellType::operator=(MasterCellType&& mct)
{
    _idx = mct._idx;
    _pin2Layer = std::move(mct._pin2Layer);
    _LayerDemand = std::move(mct._LayerDemand);
    _SameGridMC = std::move(mct._SameGridMC);
    _SameGridDemand = std::move(mct._SameGridDemand);
    _adjHGridMC = std::move(mct._adjHGridMC);
    _adjHGridDemand = std::move(mct._adjHGridDemand);
    return *this;
}

void MasterCellType::AddBlkg(unsigned layer,
                             int demand)
{
    _LayerDemand[layer] += demand;
}

void MasterCellType::reservePin(size_t s)
{
    _pin2Layer.reserve(s);
    for (unsigned i = 0; i < s; ++i)
    {
        _pin2Layer.push_back(0);
    }
}

void MasterCellType::AddPin(unsigned idx, unsigned layer)
{
    _pin2Layer[idx] = layer;
}

void MasterCellType::AddExtraSame(unsigned MC, int demand, unsigned layer)
{
    _SameGridMC[layer].push_back(MC);
    _SameGridDemand[layer].push_back(demand);
}

void MasterCellType::AddExtraadjH(unsigned MC, int demand, unsigned layer)
{
    _adjHGridMC[layer].push_back(MC);
    _adjHGridDemand[layer].push_back(demand);
}

unsigned MasterCellType::getIdx() const
{
    return _idx;
}

int MasterCellType::getLayerDemand(unsigned layer) const
{
    return _LayerDemand[layer];
}

size_t MasterCellType::getNumPins() const
{
    return _pin2Layer.size();
}

unsigned MasterCellType::getPinLayer(unsigned pin) const
{
    return _pin2Layer[pin];
}

safe::vector<unsigned> &MasterCellType::getSameGridMC(unsigned layer)
{
    return _SameGridMC[layer];
}

safe::vector<unsigned> &MasterCellType::getadjHGridMC(unsigned layer)
{
    return _adjHGridMC[layer];
}

safe::vector<int> &MasterCellType::getSameGridDemand(unsigned layer)
{
    return _SameGridDemand[layer];
}

safe::vector<int> &MasterCellType::getadjHGridDemand(unsigned layer)
{
    return _adjHGridDemand[layer];
}