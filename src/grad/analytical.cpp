/***********************************************************************

  FileName    [analytical.cpp]

  Author      [Yang Chien Yi]

  This file describes the analytical chip method.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "analytical.h"

#include <assert.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

Cost::Cost(Chip& chip) : _chip(chip) {
    unsigned dimension = chip.getNumCells() * 2;
    _f_max_val.reserve(dimension);
    _f_max_wei.reserve(dimension);
    _f_min_val.reserve(dimension);
    _f_min_wei.reserve(dimension);
    for (unsigned i = 0; i < dimension; i++) {
        _f_max_val.push_back(0);
        _f_max_wei.push_back(0);
        _f_min_val.push_back(0);
        _f_min_wei.push_back(0);
    }
}

void Cost::evaluateFG(const safe::vector<double>& x,
                      double& f,
                      safe::vector<double>& g) {
    double wirelength = 0, density = 0;
    for (unsigned i = 0; i < g.size(); i++) {
        g[i] = 0;
    }

////////////////////////////////////////////////////////////////////////
///                         WIRELENGTH                               ///
////////////////////////////////////////////////////////////////////////
#ifdef WIRELENTH_WA
    for (unsigned i = 0; i < _f_max_wei.size(); i++) {
        double val = x[i];
        _f_max_wei[i] = exp(val / gamma);
        _f_max_val[i] = _f_max_wei[i] * val;
        _f_min_wei[i] = exp(-val / gamma);
        _f_min_val[i] = _f_min_wei[i] * val;
    }
    for (unsigned i = 0; i < _chip.getNumNets(); i++)  // for every net
    {
        double x_max_val = 0, x_max_wei = 0, x_min_val = 0, x_min_wei = 0,
               y_max_val = 0, y_max_wei = 0, y_min_val = 0, y_min_wei = 0;
        GridNet& net = _chip.getNet(i);
        for (unsigned j = 0; j < net.getNumPin(); j++)  // for every pin, f
        {
            const Pin& pin = _chip.getPin(net.getPinIdx(j));
            unsigned x_idx = 2 * pin.get_cell_idx();
            unsigned y_idx = 2 * pin.get_cell_idx() + 1;
            x_max_val += _f_max_val[x_idx];
            y_max_val += _f_max_val[y_idx];
            x_max_wei += _f_max_wei[x_idx];
            y_max_wei += _f_max_wei[y_idx];
            x_min_val += _f_min_val[x_idx];
            y_min_val += _f_min_val[y_idx];
            x_min_wei += _f_min_wei[x_idx];
            y_min_wei += _f_min_wei[y_idx];
        }
        double x_max = x_max_val / x_max_wei, x_min = x_min_val / x_min_wei,
               y_max = y_max_val / y_max_wei, y_min = y_min_val / y_min_wei;
        for (unsigned j = 0; j < net.getNumPin(); j++)  // for every pin, g
        {
            const Pin& pin = _chip.getPin(net.getPinIdx(j));
            unsigned x_idx = 2 * pin.get_cell_idx();
            unsigned y_idx = 2 * pin.get_cell_idx() + 1;
            // calculate x
            g[x_idx] += (_f_max_wei[x_idx] * (1 + (x[x_idx] - x_max) / gamma) /
                         x_max_wei)  // max term
                        -
                        (_f_min_wei[x_idx] * (1 - (x[x_idx] - x_min) / gamma) /
                         x_min_wei);  // min term
            // calculate y
            g[y_idx] += (_f_max_wei[y_idx] * (1 + (x[y_idx] - y_max) / gamma) /
                         y_max_wei)  // max term
                        -
                        (_f_min_wei[y_idx] * (1 - (x[y_idx] - y_min) / gamma) /
                         y_min_wei);  // min term
        }
        wirelength += x_max - x_min + y_max - y_min;
    }
#endif
    ////////////////////////////////////////////////////////////////////////
    ///                           DENSITY                                ///
    ////////////////////////////////////////////////////////////////////////

    // assert(wirelength==0);
    f = wirelength + density;
}

void Cost::evaluateF(const safe::vector<double>& x, double& f) {
    f = Wirelength(x) + Density(x);
}

double Cost::Wirelength(const safe::vector<double>& x) {
    double ret = 0;
#ifdef WIRELENTH_WA
    for (unsigned i = 0; i < _f_max_wei.size(); i++) {
        double val = x[i];
        _f_max_wei[i] = exp(val / gamma);
        _f_max_val[i] = _f_max_wei[i] * val;
        _f_min_wei[i] = exp(-val / gamma);
        _f_min_val[i] = _f_min_wei[i] * val;
    }
    unsigned numNets = _chip.getNumNets();
    for (unsigned i = 0; i < numNets; i++)  // for every net
    {
        double x_max_val = 0, x_max_wei = 0, x_min_val = 0, x_min_wei = 0,
               y_max_val = 0, y_max_wei = 0, y_min_val = 0, y_min_wei = 0;
        GridNet& net = _chip.getNet(i);
        for (int j = 0, n = net.getNumPin(); j < n; j++)  // for every pin
        {
            const Pin& pin = _chip.getPin(net.getPinIdx(j));
            unsigned x_idx = 2 * pin.get_cell_idx();
            unsigned y_idx = 2 * pin.get_cell_idx() + 1;
            x_max_val += _f_max_val[x_idx];
            y_max_val += _f_max_val[y_idx];
            x_max_wei += _f_max_wei[x_idx];
            y_max_wei += _f_max_wei[y_idx];
            x_min_val += _f_min_val[x_idx];
            y_min_val += _f_min_val[y_idx];
            x_min_wei += _f_min_wei[x_idx];
            y_min_wei += _f_min_wei[y_idx];
        }
        ret += x_max_val / x_max_wei - x_min_val / x_min_wei +
               y_max_val / y_max_wei - y_min_val / y_min_wei;
    }
#endif
    return ret;
}

double Cost::Density(const safe::vector<double>& x) {
    double ret = 0;
    return ret;
}