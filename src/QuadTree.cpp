// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadTree.h"

QuadTree::QuadTree() noexcept
    : _NetName(""), _NetId(-1), _minLayer(-1) {
    segments.clear();
}

QuadTree::~QuadTree() noexcept {
    nodes.clear();
    pins.clear();
}

const std::string& QuadTree::get_name() const { return _NetName; }
const int QuadTree::get_net_id() const { return _NetId; }
const int QuadTree::get_min_layer() const { return _minLayer; }
const int QuadTree::get_root_idx() const { return root_idx; }

void QuadTree::add_pin(Pin* p){
    pins.push_back(p);
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
    while(operation){ // split intersected segments
        operation = false;
        for(int i = 0; i < segments.size() - 1; ++i){
            for(int j = i + 1; j < segments.size(); ++j){
                CoordPair intersection = segments[i].get_instersect(segments[j]);
                if(intersection != CoordPair(-1, -1)){
                    NetSegment splitted = segments[i].split_segment(intersection);
                    segments.push_back(splitted);
                    operation = true;
                }
            }
        }
    }
    safe::unordered_map<unsigned, CoordPair> Vertices;
    safe::unordered_map<CoordPair, unsigned> Coord2Vertices;
    safe::vector<SimpleEdge> EdgeGraph;
    // Construct vertices from pins
    unsigned pNum = 0;
    for(int i = 0; i < pins.size(); ++i){ // Add pins to Vertices
        CoordPair pinCoord(pins[i]->getRow(), pins[i]->getColumn());
        if(!Coord2Vertices.contains(pinCoord)){
            Coord2Vertices[pinCoord] = pNum;
            Vertices[pNum] = pinCoord;
            ++pNum;
        }
    } // Now we know the number of pins is pNum
    // Construct graph by segments
    unsigned vNum = pNum;
    for(int i = 0; i < segments.size(); ++i){ // Add segments to graph
        unsigned start_idx, end_idx;
        CoordPair start_coord = segments[i].get_start();
        CoordPair end_coord   = segments[i].get_end();
        if(Coord2Vertices.contains(start_coord)){
            start_idx = Coord2Vertices[start_coord];
        } else {
            Vertices[vNum] = start_coord;
            Coord2Vertices[start_coord] = vNum;
            start_idx = vNum++;
        }
        if(Coord2Vertices.contains(end_coord)){
            end_idx = Coord2Vertices[end_coord];
        } else {
            Vertices[vNum] = end_coord;
            Coord2Vertices[end_coord] = vNum;
            end_idx = vNum++;
        }
        EdgeGraph.push_back(SimpleEdge(start_idx, end_idx, segments[i].get_length()));
    }
    // Kruskal's MST algorithm
    sort(EdgeGraph.begin(), EdgeGraph.end()); // sort by length of edges
    SimpleUnionFind union_find(vNum);
    safe::vector<unsigned> selected_edges;
    safe::vector<unsigned> TreeGraph[vNum]; // adjacent list storing MST
    for(int i = 0; i < EdgeGraph.size() && selected_edges.size() < vNum - 1; ++i){
        unsigned v1 = EdgeGraph[i].get_v1(), v2 = EdgeGraph[i].get_v2();
        if(!union_find.same(v1, v2)){
            selected_edges.push_back(i);
            union_find.merge(v1, v2);
            TreeGraph[v1].push_back(v2);
            TreeGraph[v2].push_back(v1);
        }
    }
    // Remove redundant edges

    // Find the center of the tree

    // Construct quad tree


    segments.clear();
}