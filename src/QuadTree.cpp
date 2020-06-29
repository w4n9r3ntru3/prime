// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadTree.h"

QuadTree::QuadTree() noexcept
    : _NetName(""), _NetId(-1), _minLayer(-1), _maxRows(0), _maxCols(0), root_idx(-1), flag(0) {
    segments.clear();
}

QuadTree::QuadTree(std::string n, int n_id, int min_lay, int max_row, int max_col) noexcept
    : _NetName(n), _NetId(n_id), _minLayer(min_lay), _maxRows(max_row), _maxCols(max_col), root_idx(-1), flag(0) {
    segments.clear();
}

QuadTree::~QuadTree() noexcept {
    reset_tree();
}

// access to basic attributes
const std::string& QuadTree::get_name() const { return  _NetName; }
const int& QuadTree::get_net_id()       const { return    _NetId; }
const int& QuadTree::get_min_layer()    const { return _minLayer; }
const int& QuadTree::get_root_idx()     const { return  root_idx; }
bool QuadTree::is_built()               const { return root_idx != -1; }

// get nodes / pins
unsigned  QuadTree::size()              const { assert(is_built()); return nodes.size(); }
unsigned  QuadTree::pin_num()           const { assert(is_built()); return pins.size(); }
unsigned  QuadTree::pseudo_pin_num()    const { assert(is_built()); return size() - pin_num(); }
bool QuadTree::exist_node(const CoordPair& _coord) const {
    assert(is_built()); return coord2Node.contains(_coord);
}
bool QuadTree::exist_node(const unsigned _x, const unsigned _y) const {
    assert(is_built()); return coord2Node.contains(CoordPair(_x, _y));
}
bool QuadTree::is_pin(unsigned idx) const {
    assert(is_built()); return idx < pin_num();
}
bool QuadTree::is_pseudo_pin(unsigned idx) const {
    assert(is_built()); return idx >= pin_num() && idx < size();
}
int QuadTree::get_node_idx(const CoordPair& _coord) const {
    assert(is_built());
    return (coord2Node.contains(_coord)) ? coord2Node[_coord] : -1;
}
int QuadTree::get_node_idx(const unsigned _x, const unsigned _y) const {
    assert(is_built());
    return (coord2Node.contains(CoordPair(_x, _y))) ? coord2Node[CoordPair(_x, _y)] : -1;
}
QuadNode& QuadTree::get_node(unsigned idx){
    assert(is_built() && idx < size());
    return nodes[idx];
}
QuadNode& QuadTree::get_node(const CoordPair& _coord){
    int idx = get_node_idx(_coord);
    assert(is_built() && idx >= 0);
    return nodes[idx];
}
QuadNode& QuadTree::get_node(const unsigned _x, const unsigned _y){
    int idx = get_node_idx(_x, _y);
    assert(is_built() && idx >= 0);
    return nodes[idx];
}

unsigned QuadTree::get_net_length() const { return 0; }
unsigned QuadTree::get_subnet_length(unsigned idx) const { return 0; }
CoordPair QuadTree::get_bounding_box_size() const { return CoordPair(); }

int QuadTree::move_vertical(unsigned idx, int delta_x){
    increment_flag();
    return move_pin(idx, delta_x, 0);
}
int QuadTree::move_horizontal(unsigned idx, int delta_y){
    increment_flag();
    return move_pin(idx, 0, delta_y);
}

void QuadTree::optimize(unsigned max_iter = DEFAULT_OPT){
    // TODO:
}

void QuadTree::add_pin(Pin* p) { pins.push_back(p); }
void QuadTree::add_segment(int srow, int scol, int slay, 
                           int erow, int ecol, int elay){
    if(slay != elay) return; // ignore via
    segments.push_back(NetSegment(srow, scol, erow, ecol));
}
void QuadTree::construct_tree(){
    segment_to_tree();
    optimize();
}

void QuadTree::reset_tree(){
    root_idx = -1;
    flag = 0;
    nodes.clear();
    pins.clear();
    coord2Node.clear();
}

// Private functions
void QuadTree::increment_flag(){ ++flag; }
bool QuadTree::can_merge(QuadNode& n_1, QuadNode& n_2, const std::string dir) const {
    // check whether n_1 can be merged to n_2 from dir (direction)
    unsigned direction = dir2Num(dir), opp_direction = opposite_dir(dir);
    assert(direction >= 1 && direction <= 4);
    if(direction == 2 || direction == 4) return can_merge(n_2, n_1, num2Dir(opp_direction));
    if(direction == 1){
        assert(n_2.get_up() == n_1.get_self());
        return !(n_1.has_left() && n_2.has_left()) && !(n_1.has_right() && n_2.has_right());
    } else { // direction == 3
        assert(n_2.get_left() == n_1.get_self());
        return !(n_1.has_up() && n_2.has_up()) && !(n_1.has_down() && n_2.has_down());
    }
}
void QuadTree::merge_nodes(QuadNode& n_1, QuadNode& n_2, const std::string dir){
    // merge node n_1 to n_2 from dir (direction)
    assert(can_merge(n_1, n_2, dir));
    unsigned direction = dir2Num(dir), opp_direction = opposite_dir(dir);
    assert(direction >= 1 && direction <= 4);
    if(direction == 1 || direction == 2){
        if(!n_2.has_left())  n_2.set_left(n_1.get_left());
        if(!n_2.has_right()) n_2.set_right(n_1.get_right());

    } else { // direction == 3 or 4
        // TODO:
    }
}
void QuadTree::insert_node(){
    // TODO:
}
void QuadTree::delete_node(){
    // TODO:
}
inline int QuadTree::move_pin(unsigned idx, int delta_x, int delta_y){
    if(idx < 0) return 0;
    assert(is_built() && idx < size());
    assert(delta_x * delta_y == 0); // move in one direction at a time
    if(nodes[idx].get_flag() == flag) return 0;
    delta_x = clamp(delta_x, 0 - nodes[idx].get_coord_x(), _maxRows - nodes[idx].get_coord_x());
    delta_y = clamp(delta_y, 0 - nodes[idx].get_coord_y(), _maxCols - nodes[idx].get_coord_y());
    if(delta_x == 0 && delta_y == 0) return 0;
    
    bool direction = delta_x != 0; // true if direction is vertical else horizontal
    int total_movement = 0;
    if(direction){ // vertical
        if(nodes[idx].has_left()){ // look at the left side
            int left = nodes[idx].get_left();
            if(delta_x < 0){ // move upwards
                if(nodes[idx].has_up()){ // consider the upper node
                    int up = nodes[idx].get_up();
                    unsigned distance = nodes[idx].dist(nodes[up].get_coord());
                    if(distance < (unsigned)ABS(delta_x)){
                        nodes[idx].move_vertical(delta_x);
                    } else {
                        if(!nodes[up].has_left()){

                        } else {

                        }
                    }
                } else {

                }
            } else { // delta_x > 0, move downwards

            }
            nodes[idx].update_flag(flag);
            total_movement += move_pin(left, delta_x, 0);
        } else {

        }
    } else {       // horizontal

    }
}

void QuadTree::self_optimize(){

}

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
    assert(pNum == pins.size());
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
    dfs_construct_tree(SimpleTree, Vertices, new_idx_mapping, found_center, -1);
    
    // Final preperation
    root_idx = new_idx_mapping[found_center];
    assert(nodes[root_idx].get_parent() == -1);
    for(size_t i = 0; i < nodes.size(); ++i){ // set coordinate to node index mapping
        coord2Node[nodes[i].get_coord()] = i;
    }

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
        if((int)SimpleTree[now][i] != parent){
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
