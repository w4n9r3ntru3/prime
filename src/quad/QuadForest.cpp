// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "../include/QuadForest.h"

QuadForest::QuadForest() noexcept {}
QuadForest::QuadForest(Chip& chip) noexcept {
    construct_forest(chip);
}
// QuadForest::~QuadForest() noexcept {
//     clear();
// }

void QuadForest::construct_forest(Chip& chip) {
    // Get basic information
    baseRowIdx = chip.getRowBase();
    baseColIdx = chip.getColumnBase();
    maxRows    = chip.getNumRows();
    maxCols    = chip.getNumColumns();
    maxLayers  = chip.getNumLayers();

    size_t numPins = chip.getNumPins();
    pinIdx2Tree = safe::vector<unsigned>(numPins);
    // std::cout << "Rows: " << maxRows << " Columns: " << maxCols << " Layers: " << maxLayers << std::endl;

    // Get net segments from Chip
    size_t num_nets = chip.getNumNets();
    for(size_t i = 0; i < num_nets; ++i){
        GridNet& net = chip.getNet(i); // get net i
        // std::string net_name     = net.getName();
        unsigned    net_id       = net.getIdx();
        unsigned    net_minlayer = net.getMinlayer();

        QuadTree qt(net_id, net_minlayer, baseRowIdx, baseColIdx, maxRows, maxCols);
        // Get pins
        size_t pin_num = net.getNumPin();
        for(size_t j = 0; j < pin_num; ++j){
            const Pin& pin = chip.getPin(net, j);
            unsigned pin_idx = pin.getIdx();
            unsigned pin_row = chip.getPinRow(pin);
            unsigned pin_col = chip.getPinColumn(pin);
            unsigned pin_lay = pin.getLayer();
            pinIdx2Tree[pin_idx] = i;
            qt.add_pin(SimplePin(pin_idx, pin_row, pin_col, pin_lay)); // TODO: pin: (index, layer, row, column)
        }
        // Get segments
        safe::vector<unsigned>& segments = net.getSegments();
        assert(segments.size() % 6 == 0);
        for(size_t j = 0; j < segments.size(); j += 6){
            qt.add_segment(segments[j], segments[j+1], segments[j+2], segments[j+3], segments[j+4], segments[j+5]);
        }
        qt.construct_tree();

        qtrees.push_back(qt);
        // name2NetIdx[net_name] = qtrees.size() - 1;
    }
}

size_t QuadForest::size() const { return qtrees.size(); }
QuadTree& QuadForest::get_tree(size_t idx) {
    assert(idx < qtrees.size());
    return qtrees[idx];
}
// QuadTree& QuadForest::get_tree(std::string s) {
//     assert(name2NetIdx.contains(s));
//     return qtrees[name2NetIdx.at(s)];
// }
QuadTree& QuadForest::operator[](size_t idx){
    assert(idx < qtrees.size());
    return qtrees[idx];
}
const QuadTree& QuadForest::operator[](size_t idx) const {
    assert(idx < qtrees.size());
    return qtrees[idx];
}

void QuadForest::push_back(const QuadTree& qt) {
    qtrees.push_back(qt);
}
void QuadForest::push_back(QuadTree&& qt) {
    qtrees.push_back(std::move(qt));
}

// void QuadForest::clear(){
//     qtrees.clear();
//     maxRows = maxCols = maxLayers = 0;
//     name2NetIdx.clear();
//     for(size_t i = 0; i < pins.size(); ++i){
//         pins[i].reset();
//     }
//     pins.clear();
//     pins_in_tree.clear();
// }

void QuadForest::return_segments(Chip& chip) {
    for(size_t i = 0; i < qtrees.size(); ++i) {
        GridNet& net = chip.getNet(i);
        qtrees[i].convert_to_segments();
        safe::vector<NetSegment>& segments = qtrees[i].get_segments();
        net.clearSegments();
        for(size_t j = 0; j < segments.size(); ++j){
            net.addSegment(segments[j].get_xs(), segments[j].get_ys(), segments[j].get_layer(), 
                           segments[j].get_xe(), segments[j].get_ye(), segments[j].get_layer_end());
        }
    }
}

std::ostream& operator<<(std::ostream& out, QuadForest& qf) {
    size_t numRoutes = 0;
    for(size_t i = 0; i < qf.size(); ++i){
        numRoutes += qf[i].size();
    }
    out << "NumRoutes " << numRoutes << std::endl;
    for(size_t i = 0; i < qf.size(); ++i){
        qf[i].convert_to_segments();
        out << qf[i];
    }
    return out;
}