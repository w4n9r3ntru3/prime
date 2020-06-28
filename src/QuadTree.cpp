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

const std::string& QuadTree::get_name() const { return  _NetName; }
const int& QuadTree::get_net_id()       const { return    _NetId; }
const int& QuadTree::get_min_layer()    const { return _minLayer; }
const int& QuadTree::get_root_idx()     const { return  root_idx; }

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
    bool operation = true;
    while(operation){ // merge overlapping segments
        operation = false;
        for(size_t i = 0; i < segments.size() - 1; ++i){
            for(size_t j = i + 1; j < segments.size(); ++j){
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
        for(size_t i = 0; i < segments.size() - 1; ++i){
            for(size_t j = i + 1; j < segments.size(); ++j){
                CoordPair intersection = segments[i].get_instersect(segments[j]);
                if(intersection != CoordPair(-1, -1)){
                    NetSegment splitted_1 = segments[i].split_segment(intersection);
                    NetSegment splitted_2 = segments[j].split_segment(intersection);
                    if(splitted_1 != NetSegment()) segments.push_back(splitted_1);
                    if(splitted_2 != NetSegment()) segments.push_back(splitted_2);
                    operation = true;
                }
            }
        }
    }
    safe::unordered_map<unsigned, CoordPair> Vertices;
    safe::map<CoordPair, unsigned> Coord2Vertices;
    safe::vector<SimpleEdge> EdgeGraph;
    // Construct vertices from pins
    unsigned pNum = 0;
    for(size_t i = 0; i < pins.size(); ++i){ // Add pins to Vertices
        CoordPair pinCoord(pins[i]->getRow(), pins[i]->getColumn());
        if(!Coord2Vertices.contains(pinCoord)){
            Coord2Vertices[pinCoord] = pNum;
            Vertices[pNum] = pinCoord;
            ++pNum;
        }
    } // Now we know the number of pins is pNum
    // Construct graph by segments
    unsigned vNum = pNum;
    for(size_t i = 0; i < segments.size(); ++i){ // Add segments to graph
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
    // Kruskal's MST algorithm O(ElogV + E)
    sort(EdgeGraph.begin(), EdgeGraph.end()); // sort by length of edges
    SimpleUnionFind union_find(vNum);
    safe::vector<int> selected_edges(EdgeGraph.size());
    safe::vector<VEPair> TreeGraph[vNum]; // adjacent list for storing MST (unweighted)
    for(size_t i = 0; i < EdgeGraph.size(); ++i){
        unsigned v1 = EdgeGraph[i].get_v1(), v2 = EdgeGraph[i].get_v2();
        if(!union_find.same(v1, v2)){
            selected_edges[i] = 1;
            union_find.merge(v1, v2);
            TreeGraph[v1].push_back(VEPair(v2, i));
            TreeGraph[v2].push_back(VEPair(v1, i));
        }
    }
    union_find.clear();
    // Remove redundant edges O(E)
    dfs_tree_graph(TreeGraph, selected_edges, vNum, pNum, 0, 0, 0);
    // Reconstruct pruned tree O(E)
    for(size_t i = 0; i < vNum; ++i) TreeGraph[i].clear();
    safe::vector<unsigned> SimpleTree[vNum];
    unsigned tree_size = 1; // tree size = # of vertices = # of edges + 1
    for(size_t i = 0; i < EdgeGraph.size(); ++i){
        if(selected_edges[i]){
            unsigned v1 = EdgeGraph[i].get_v1(), v2 = EdgeGraph[i].get_v2();
            SimpleTree[v1].push_back(v2);
            SimpleTree[v2].push_back(v1);
            ++tree_size;
        }
    }

    // Find the center of the tree
    safe::vector<double> vertex_rank(vNum, -1.0);
    dfs_tree_center(SimpleTree, vertex_rank, tree_size, 0, 0);
    unsigned found_center = 0;
    double min_score = DINF + 1.0;
    for(size_t i = 0; i < vNum; ++i){
        if(vertex_rank[i] > -EPS && vertex_rank[i] < min_score){
            min_score = vertex_rank[i];
            found_center = i;
        }
    }

    // Translate vertex indices to new indices
    safe::vector<int> new_idx_mapping(vNum); // maps old indices to new ones
    unsigned new_idx = 0;
    for(size_t i = 0; i < vNum; ++i){
        if(vertex_rank[i] > -EPS){
            new_idx_mapping[i] = new_idx++;
        } else {
            new_idx_mapping[i] = -1;
        }
    }
    assert(new_idx == tree_size);

    // Construct quad nodes
    for(size_t i = 0; i < new_idx; ++i){
        nodes.push_back(QuadNode(i));
    }
    // Construct quad tree
    dfs_construct_tree(SimpleTree, Vertices, new_idx_mapping, 0, -1);

    segments.clear();
}

inline bool QuadTree::dfs_tree_graph(
        safe::vector<VEPair> TreeGraph[], 
        safe::vector<int>& selected_edges, 
        const unsigned vNum, 
        const unsigned pNum, 
        const unsigned now, 
        const unsigned parent, 
        const unsigned edge_idx){
    // Use DFS to remove redundant edges O(E)
    // If the end of a path is not a pin then some edges of this path are redundant.
    bool has_pin = false;
    for(size_t i = 0; i < TreeGraph[now].size(); ++i){
        if(TreeGraph[now][i].first == parent) continue;
        has_pin = has_pin || dfs_tree_graph(TreeGraph, selected_edges, 
                                            vNum, pNum, 
                                            TreeGraph[now][i].first, now, 
                                            TreeGraph[now][i].second);
    }
    if(now < pNum || has_pin) return true;
    else {
        selected_edges[edge_idx] = false;
        return false;
    }
}

inline unsigned QuadTree::dfs_tree_center(
        safe::vector<unsigned> SimpleTree[],
        safe::vector<double>&  vertex_rank, 
        const unsigned tree_size,
        const unsigned now, 
        const unsigned parent){
    // Use DFS to find the center of the tree O(E)
    safe::vector<unsigned> num_vertices;
    for(size_t i = 0; i < SimpleTree[now].size(); ++i){
        if(SimpleTree[now][i] != parent){
            num_vertices.push_back(
                dfs_tree_center(SimpleTree, vertex_rank, tree_size, SimpleTree[now][i], now)
            );
        }
    }
    int num_branch = 0;
    if(now != parent){ // first DFS node will be now = parent = 0
        num_branch = 1 + num_vertices.size();
    } else {
        num_branch = num_vertices.size();
    }
    if(num_branch == 0){ // only happens when there is only one node in the tree
        vertex_rank[now] = 0.0;
        return 0;
    }
    if(num_branch == 1){ // leaf node
        vertex_rank[now] = DINF;
        return 1;
    }
    double score = 0.0;
    unsigned total = 0;
    double average_score = ((double)tree_size - 1.0) / (double)num_branch;
    for(size_t i = 0; i < num_vertices.size(); ++i){
        total += num_vertices[i];
        score += ((double)num_vertices[i] - average_score) *
                 ((double)num_vertices[i] - average_score);
    }
    score += ((double)(tree_size - total - 1) - average_score) * 
             ((double)(tree_size - total - 1) - average_score);
    score /= (double)num_branch; // == variance
    vertex_rank[now] = score;
    return total + 1;
}

inline void QuadTree::dfs_construct_tree(
        safe::vector<unsigned> SimpleTree[],
        safe::unordered_map<unsigned, CoordPair>& Vertices,
        safe::vector<int>& new_idx_mapping,
        const unsigned now, 
        const int parent){
    // Use DFS to construct quad tree
    nodes[new_idx_mapping[now]].set_parent(parent);
    nodes[new_idx_mapping[now]].reset_coord(Vertices[now]);
    for(size_t i = 0; i < SimpleTree[now].size(); ++i){
        if((int)SimpleTree[now][i] != parent){
            unsigned direction = check_direction(Vertices[now], Vertices[SimpleTree[now][i]]);
            if(direction == 1){        // up
                nodes[new_idx_mapping[now]].set_up(new_idx_mapping[SimpleTree[now][i]]);
            } else if(direction == 2){ // down
                nodes[new_idx_mapping[now]].set_down(new_idx_mapping[SimpleTree[now][i]]);
            } else if(direction == 3){ // left
                nodes[new_idx_mapping[now]].set_left(new_idx_mapping[SimpleTree[now][i]]);
            } else if(direction == 4){ // right
                nodes[new_idx_mapping[now]].set_right(new_idx_mapping[SimpleTree[now][i]]);
            }
            dfs_construct_tree(SimpleTree, Vertices, new_idx_mapping, SimpleTree[now][i], now);
        }
    }
}

unsigned QuadTree::check_direction(const CoordPair c_1, const CoordPair c_2) const {
    // find the relative direction between the two vertices
    if(c_1.first <  c_2.first && c_1.second == c_2.second) return 1; // up
    if(c_1.first >  c_2.first && c_1.second == c_2.second) return 2; // down
    if(c_1.first == c_2.first && c_1.second <  c_2.second) return 3; // left
    if(c_1.first == c_2.first && c_1.second >  c_2.second) return 4; // right
    assert(false);
    return 0;
}