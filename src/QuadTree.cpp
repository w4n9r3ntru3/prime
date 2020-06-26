// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadTree.h"

QuadTree::QuadTree() noexcept
    : _NetName(""), _NetId(-1), _minLayer(-1) {
    segments.clear();
}

const std::string& QuadTree::get_name() const { return _NetName; }
const int QuadTree::get_net_id() const { return _NetId; }
const int QuadTree::get_min_layer() const { return _minLayer; }
const int QuadTree::get_root_idx() const { return root_idx; }

void QuadTree::add_pin(Pin* p){
    // TODO:
}

void QuadTree::add_segment(int srow, int scol, int slay, 
                           int erow, int ecol, int elay){
    if(slay != elay) return; // ignore via
    segments.push_back(NetSegment(srow, scol, erow, ecol));
}

void QuadTree::construct_tree(){
    // TODO:
    segment_to_tree();
}

// Private functions
void QuadTree::segment_to_tree(){
    // TODO: need to sort the segments?
    // sort(segments->begin(), segments->end()); // sort the segments by coordinates
    safe::unordered_map<CoordPair, unsigned> Coord2Idx;

    bool operation = true;
    while(operation){ // merge overlapping segments
        operation = false;
        for(int i = 0; i < segments.size() - 1; ++i){
            for(int j = i + 1; j < segments.size(); ++j){
                if(segments[i].check_overlap(segments[j])){
                    segments[i].merge_segment(segments[j]);
                    segments[j] = segments[segments.size() - 1];
                    segments.pop_back();
                    --j;
                    operation = true;
                }
            }
        }
    } operation = true;
    while(operation){
        operation = false;
        for(int i = 0; i < segments.size() - 1; ++i){
            for(int j = i + 1; j < segments.size(); ++j){
                if(segments[i].check_overlap(segments[j])){
                    segments[i].merge_segment(segments[j]);
                    segments[j] = segments[segments.size() - 1];
                    segments.pop_back();
                    --j;
                    operation = true;
                }
            }
        }
    }

    segments.clear();
}