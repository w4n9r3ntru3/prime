// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadForest.h"

QuadForest::QuadForest() noexcept {}
QuadForest::QuadForest(Chip& chip) noexcept {
    construct_forest(chip);
}

void QuadForest::construct_forest(Chip& chip) {
    // Get basic information
    maxRows   = chip.getNumRows();
    maxCols   = chip.getNumColumns();
    maxLayers = chip.getNumLayers();

    // Get net segments from Chip
    size_t num_nets = chip.getNumNets();
    for(size_t i = 0; i < num_nets; ++i){
        GridNet& net = chip.getNet(i); // get net i
        std::string net_name     = net.getName();
        unsigned    net_id       = net.getId();
        unsigned    net_minlayer = net.getMinlayer();

        QuadTree qt(net_name, net_id, net_minlayer, maxRows, maxCols);
        // Get pins
        size_t pin_num = net.getNumPin();
        for(size_t j = 0; j < pin_num; ++j){
            qt.add_pin(net.getPinPtr(j));
        }
        // Get segments
        safe::vector<unsigned>& segments = net.getSegments();
        assert(segments.size() % 6 == 0);
        for(size_t j = 0; j < segments.size(); j += 6){
            qt.add_segment(segments[j], segments[j+1], segments[j+2], segments[j+3], segments[j+4], segments[j+5]);
        }
        qt.construct_tree();

        qtrees.push_back(qt);
        name2NetIdx[net_name] = qtrees.size() - 1;
    }
}

unsigned QuadForest::size() const { return qtrees.size(); }
QuadTree& QuadForest::get_tree(unsigned idx) {
    assert(idx < qtrees.size());
    return qtrees[idx];
}
QuadTree& QuadForest::get_tree(std::string s) {
    assert(name2NetIdx.contains(s));
    return qtrees[name2NetIdx.at(s)];
}

void QuadForest::push_back(const QuadTree& qt) {
    qtrees.push_back(qt);
}
void QuadForest::push_back(QuadTree&& qt) {
    qtrees.push_back(std::move(qt));
}