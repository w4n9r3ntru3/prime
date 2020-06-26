#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "die.h"

#ifndef ROUTER
#define ROUTER

class Router {
   public:
    Router(std::fstream& inputfile, std::fstream& outputfile)
        : _outputfile(outputfile), _err(0), _cost(0) {
        // read inputfile
        int x, y, z;
        inputfile >> x >> y >> z;
        unsigned x1, y1, z1, x2, y2, z2;
        _die = new Die(x, y, z, Manhattan);
        while (inputfile >> x1 >> y1 >> z1 >> x2 >> y2 >> z2) {
            unsigned idx1 = _die->get_idx(x1, y1, z1);
            unsigned idx2 = _die->get_idx(x2, y2, z2);
            _NetList.push_back(idx1);
            if (!_die->add_net(idx1, idx2)) {
                std::cerr << "invalid coordinate: " << x1 << " " << y1 << " "
                          << z1 << " " << x2 << " " << y2 << " " << z2
                          << std::endl;
                _err = true;
            }
        }
        _die->add_normal();
        inputfile.close();
    }
    void rout() {
        for (int i = 0; i < _NetList.size(); i++) {
            IdxList ans;
            _cost += _die->A_star(_NetList[i], ans);
            for (int j = 0; j < ans.size(); j++) {
                _outputfile << _die->get_x(ans[j]) << " " << _die->get_y(ans[j])
                            << " " << _die->get_z(ans[j]) << std::endl;
            }
            _outputfile << std::endl;
        }
        std::cout << "total cost: " << _cost << std::endl;
        _outputfile.close();
    }
    bool err() const { return _err; }

   private:
    Die* _die;
    IdxList _NetList;
    std::fstream& _outputfile;
    bool _err;
    int _cost;
};

#endif