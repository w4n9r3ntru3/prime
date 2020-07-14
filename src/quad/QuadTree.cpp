// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadTree.h"

#include <functional>

// QuadTree::QuadTree() noexcept
//     : _NetId(-1),
//       _baseRow(-1),
//       _baseCol(-1),
//       _minLayer(-1),
//       _maxRows(0),
//       _maxCols(0),
//       _maxLayers(-1),
//       root_idx(-1),
//       flag(0) {
//     segments.clear();
// }

QuadTree::QuadTree(Chip& chip,
                   int n_id,
                   int min_lay,
                   int base_row,
                   int base_col,
                   int max_row,
                   int max_col,
                   int max_layer) noexcept
    : chip(chip),
      in_the_air(false),
      _NetId(n_id),
      _baseRow(base_row),
      _baseCol(base_col),
      _minLayer(min_lay),
      _maxRows(max_row),
      _maxCols(max_col),
      _maxLayers(max_layer),
      root_idx(-1),
      flag(0) {
    segments.clear();
}

// QuadTree::~QuadTree() noexcept {
//     reset_tree();
// }

enum class Direction { Up, Down, Left, Right };

// TODO: update cap within the same cell
template <bool ripup>
static inline void modify_self_capacity(Chip& chip, QuadNode& qn) {
    int min_lay = -1, max_lay = -1;

    assert(max_lay >= 0);
    assert(min_lay >= 0);
    int span = max_lay - min_lay;

    // TODO: update the nets
    int positions[4] = {};
}

// TODO: update cap in between cells
template <bool ripup, Direction dir>
static inline void modify_edge_capacity(Chip& chip,
                                        QuadTree& qt,
                                        QuadNode& qn) {
    QuadNode other;
    int dist;

    // TODO: in the for loop update the cost
    switch (dir) {
        case Direction::Up:
            other = qt.get_nodes()[qn.get_up()];
            dist = other.get_coord_y() - qn.get_coord_y();
            for (int i = 0; i < dist; ++i) {
            }
            break;
        case Direction::Down:
            other = qt.get_nodes()[qn.get_down()];
            dist = qn.get_coord_y() - other.get_coord_y();
            for (int i = 0; i < dist; ++i) {
            }
            break;
        case Direction::Left:
            other = qt.get_nodes()[qn.get_left()];
            dist = qn.get_coord_x() - other.get_coord_x();
            for (int i = 0; i < dist; ++i) {
            }
            break;
        case Direction::Right:
            other = qt.get_nodes()[qn.get_right()];
            dist = other.get_coord_x() - qn.get_coord_x();
            for (int i = 0; i < dist; ++i) {
            }
            break;
    }
    assert(dist >= 0);
}

template <bool ripup>
void dfs_rip_or_put(Chip& chip, QuadTree& qt, QuadNode& qn, Direction indir) {
    // TODO: handle rip up and put down

    modify_self_capacity<ripup>(chip, qn);

    if (indir != Direction::Down && qn.has_up()) {
        modify_edge_capacity<ripup, Direction::Up>(chip, qt, qn);
        dfs_rip_or_put<ripup>(chip, qt.get_nodes()[qn.get_up()], Direction::Up);
    }

    if (indir != Direction::Up && qn.has_down()) {
        modify_edge_capacity<ripup, Direction::Down>(chip, qt, qn);
        dfs_rip_or_put<ripup>(chip, qt.get_nodes()[qn.get_down()], Direction::Down);
    }

    if (indir != Direction::Right && qn.has_left()) {
        modify_edge_capacity<ripup, Direction::Left>(chip, qt, qn);
        dfs_rip_or_put<ripup>(chip, qt.get_nodes()[qn.get_left()], Direction::Left);
    }

    if (indir != Direction::Left && qn.has_right()) {
        modify_edge_capacity<ripup, Direction::Right>(chip, qt, qn);
        dfs_rip_or_put<ripup>(chip, qt.get_nodes()[qn.get_right()], Direction::Right);
    }
}

void QuadTree::ripup(void) {
    assert(!in_the_air);

    in_the_air = true;
}

void QuadTree::putdown(void) {
    assert(in_the_air);

    in_the_air = false;
}

// access to basic attributes
// std::string QuadTree::get_name() const { return  _NetName; }
int QuadTree::get_net_id() const {
    return _NetId;
}
int QuadTree::get_min_layer() const {
    return _minLayer;
}
int QuadTree::get_root_idx() const {
    return root_idx;
}
bool QuadTree::is_built() const {
    return root_idx != -1;
}

// get nodes / pins
unsigned QuadTree::size() const {
    assert(is_built());
    return nodes.size();
}
unsigned QuadTree::pin_num() const {
    assert(is_built());
    return pins.size();
}
unsigned QuadTree::pseudo_pin_num() const {
    assert(is_built());
    return size() - pin_num();
}
bool QuadTree::exist_node(const CoordPair& _coord) const {
    assert(is_built());
    return coord2Node.contains(_coord);
}
bool QuadTree::exist_node(const unsigned _x, const unsigned _y) const {
    assert(is_built());
    return coord2Node.contains(CoordPair(_x, _y));
}
bool QuadTree::is_pin(unsigned idx) const {
    assert(is_built());
    return idx < pin_num();
}
bool QuadTree::is_pseudo_pin(unsigned idx) const {
    assert(is_built());
    return idx >= pin_num() && idx < size();
}
int QuadTree::get_node_idx(const CoordPair& _coord) const {
    assert(is_built());
    return (coord2Node.contains(_coord)) ? coord2Node[_coord] : -1;
}
int QuadTree::get_node_idx(const unsigned _x, const unsigned _y) const {
    assert(is_built());
    return (coord2Node.contains(CoordPair(_x, _y)))
               ? coord2Node[CoordPair(_x, _y)]
               : -1;
}
QuadNode& QuadTree::get_node(unsigned idx) {
    assert(is_built() && idx < size());
    return nodes[idx];
}
QuadNode& QuadTree::get_node(const CoordPair& _coord) {
    int idx = get_node_idx(_coord);
    assert(is_built() && idx >= 0);
    return nodes[idx];
}
QuadNode& QuadTree::get_node(const unsigned _x, const unsigned _y) {
    int idx = get_node_idx(_x, _y);
    assert(is_built() && idx >= 0);
    return nodes[idx];
}
safe::vector<SimplePin>& QuadTree::get_pin_list() {
    return pins;
}
safe::vector<NetSegment>& QuadTree::get_segments() {
    return segments;
}

unsigned QuadTree::get_net_length() const {
    return 0;
}
unsigned QuadTree::get_subnet_length(unsigned idx) const {
    assert(idx < nodes.size());
    return 0;
}
CoordPair QuadTree::get_bounding_box_size() const {
    return CoordPair();
}

int QuadTree::move_vertical(unsigned idx, int delta_x) {
    increment_flag();
    return move_pin(idx, delta_x, 0);
}
int QuadTree::move_horizontal(unsigned idx, int delta_y) {
    increment_flag();
    return move_pin(idx, 0, delta_y);
}

// void QuadTree::optimize(unsigned max_iter = DEFAULT_OPT){
//     // TODO:
// }

void QuadTree::add_pin(SimplePin p) {
    pins.push_back(p);
}
void QuadTree::add_segment(int srow,
                           int scol,
                           int slay,
                           int erow,
                           int ecol,
                           int elay) {
    if (slay == elay) {
        segments.push_back(NetSegment(srow, scol, erow, ecol, slay));
    } else {
        vias.push_back(NetSegment(srow, scol, erow, ecol, slay));
    }
}
void QuadTree::construct_tree() {
    segment_to_tree();
    // optimize();
}
void QuadTree::convert_to_segments() {
    tree_to_segment();
}
void QuadTree::reset_tree() {
    root_idx = -1;
    flag = 0;
    nodes.clear();
    pins.clear();
    coord2Node.clear();
}

// Private functions
void QuadTree::increment_flag() {
    ++flag;
}
bool QuadTree::can_merge(QuadNode& n_1,
                         QuadNode& n_2,
                         const std::string dir) const {
    // FIXME:
    // check whether n_1 can be merged to n_2 from dir (direction)
    unsigned direction = dir2Num(dir), opp_direction = opposite_dir(dir);
    assert(direction >= 1 && direction <= 4);
    if (direction == 2 || direction == 4)
        return can_merge(n_2, n_1, num2Dir(opp_direction));
    if (direction == 1) {
        assert(n_2.get_up() == n_1.get_self());
        return !(n_1.has_left() && n_2.has_left()) &&
               !(n_1.has_right() && n_2.has_right());
    } else {  // direction == 3
        assert(n_2.get_left() == n_1.get_self());
        return !(n_1.has_up() && n_2.has_up()) &&
               !(n_1.has_down() && n_2.has_down());
    }
}
void QuadTree::merge_nodes(QuadNode& n_1,
                           QuadNode& n_2,
                           const std::string dir) {
    // FIXME:
    // merge node n_1 to n_2 from dir (direction)
    assert(can_merge(n_1, n_2, dir));
    unsigned direction = dir2Num(dir), opp_direction = opposite_dir(dir);
    assert(direction >= 1 && direction <= 4 && opp_direction >= 1 &&
           opp_direction <= 4);
    if (direction == 1 || direction == 2) {
        if (!n_2.has_left())
            n_2.set_left(n_1.get_left());
        if (!n_2.has_right())
            n_2.set_right(n_1.get_right());

    } else {  // direction == 3 or 4
        // TODO:
    }
    // TODO: coord2Node
}
void QuadTree::insert_node() {
    // TODO:
}
void QuadTree::delete_node() {
    // TODO:
}
inline int QuadTree::move_pin(unsigned idx, int delta_x, int delta_y) {
    // FIXME:
    assert(is_built() && idx < size());
    assert(delta_x * delta_y == 0);  // move in one direction at a time
    if (nodes[idx].get_flag() == flag)
        return 0;
    delta_x = clamp(delta_x, 0 - nodes[idx].get_coord_x(),
                    _maxRows - nodes[idx].get_coord_x());
    delta_y = clamp(delta_y, 0 - nodes[idx].get_coord_y(),
                    _maxCols - nodes[idx].get_coord_y());
    if (delta_x == 0 && delta_y == 0)
        return 0;

    bool direction =
        delta_x != 0;  // true if direction is vertical else horizontal
    int total_movement = 0;
    if (direction) {                  // vertical
        if (nodes[idx].has_left()) {  // look at the left side
            int left = nodes[idx].get_left();
            if (delta_x < 0) {              // move upwards
                if (nodes[idx].has_up()) {  // consider the upper node
                    int up = nodes[idx].get_up();
                    unsigned distance = nodes[idx].dist(nodes[up].get_coord());
                    if (distance < (unsigned)ABS(delta_x)) {
                        nodes[idx].move_vertical(delta_x);
                        total_movement += delta_x;
                    } else if (distance == (unsigned)ABS(delta_x)) {
                        if (can_merge(nodes[idx], nodes[up], "down")) {
                            merge_nodes(nodes[idx], nodes[up], "down");
                        } else {  // cannot merge
                            delta_x = nodes[up].get_coord_x() - 1 -
                                      nodes[idx].get_coord_x();
                            return move_pin(idx, delta_x, 0);
                        }
                    } else {
                    }
                } else {  // no upper node -> limit = 0
                }
            } else {  // delta_x > 0, move downwards
            }
            nodes[idx].update_flag(flag);
            total_movement += move_pin(left, delta_x, 0);
        } else {
        }
    } else {  // horizontal
    }
    return 0;
}

// void QuadTree::self_optimize(){

// }

void QuadTree::segment_to_tree() {
    // print_segments();
    safe::unordered_map<unsigned, CoordPair> Vertices;
    safe::map<CoordPair, unsigned> Coord2Vertices;
    safe::map<CoordPair, bool> HasVia;
    safe::vector<SimpleEdge> EdgeGraph;
    safe::unordered_map<unsigned, int> VertexLayer;
    // Construct vertices from pins
    unsigned pNum = 0;
    pinIdx2Node.clear();
    for (size_t i = 0; i < pins.size(); ++i) {  // Add pins to Vertices
        CoordPair pinCoord(pins[i].get_row(), pins[i].get_col());
        // std::cerr << "pin " << i << " (" << pinCoord.first << ", " <<
        // pinCoord.second << ", " << pins[i].get_layer() << ")\n";
        if (!Coord2Vertices.contains(
                pinCoord)) {  // Assume that all pins have different coordinates
            Coord2Vertices[pinCoord] = pNum;
            Vertices[pNum] = pinCoord;
            VertexLayer[pNum] = pins[i].get_layer();
            pinIdx2Node[pins[i].get_idx()] = pNum;
            // std::cerr << "Pin " << pNum << " : " << pinCoord.first << ", " <<
            // pinCoord.second << "\n";
            ++pNum;
        } else {  // Different pins with same x, y coordinates but on different
                  // layers
            // std::cerr << "Repeated pin " << i << " " << pinCoord.first << ",
            // " << pinCoord.second << "\n";
            pinIdx2Node[pins[i].get_idx()] = Coord2Vertices[pinCoord];
        }
    }  // Now we know the number of pins is pNum
    // assert(pins.size() == pins2NodeIdx.size());

    for (size_t i = 0; i < vias.size(); ++i) {
        if (!HasVia.contains(vias[i].get_start())) {
            HasVia[vias[i].get_start()] = true;
        }
    }

    bool operation = true;
    while (operation && segments.size() > 0) {  // merge overlapping segments
        operation = false;
        for (size_t i = 0; i < segments.size() - 1; ++i) {
            for (size_t j = i + 1; j < segments.size(); ++j) {
                if (segments[i].check_overlap(segments[j])) {
                    int importance_i = 0;
                    int importance_j = 0;
                    size_t original_total_length =
                        segments[i].get_length() + segments[j].get_length();
                    size_t new_total_length = 0;
                    if (Coord2Vertices.contains(segments[i].get_start()) &&
                        segments[i].get_layer() ==
                            VertexLayer
                                [Coord2Vertices[segments[i].get_start()]])
                        ++importance_i;
                    if (Coord2Vertices.contains(segments[i].get_end()) &&
                        segments[i].get_layer() ==
                            VertexLayer[Coord2Vertices[segments[i].get_end()]])
                        ++importance_i;
                    if (Coord2Vertices.contains(segments[j].get_start()) &&
                        segments[j].get_layer() ==
                            VertexLayer
                                [Coord2Vertices[segments[j].get_start()]])
                        ++importance_j;
                    if (Coord2Vertices.contains(segments[j].get_end()) &&
                        segments[j].get_layer() ==
                            VertexLayer[Coord2Vertices[segments[j].get_end()]])
                        ++importance_j;
                    // if(HasVia.contains(segments[i].get_start()))
                    // ++importance_i;
                    // if(HasVia.contains(segments[i].get_end()))
                    // ++importance_i;
                    // if(HasVia.contains(segments[j].get_start()))
                    // ++importance_j;
                    // if(HasVia.contains(segments[j].get_end()))
                    // ++importance_j;
                    std::tuple<NetSegment, NetSegment, NetSegment>
                        segment_tuple =
                            merge_segments(segments[i], segments[j],
                                           importance_i >= importance_j);
                    new_total_length = std::get<0>(segment_tuple).get_length() +
                                       std::get<1>(segment_tuple).get_length() +
                                       std::get<2>(segment_tuple).get_length();
                    if (new_total_length >= original_total_length)
                        continue;
                    if (std::get<0>(segment_tuple).get_length() > 0) {
                        segments.push_back(std::get<0>(segment_tuple));
                    }
                    if (std::get<1>(segment_tuple).get_length() > 0) {
                        segments.push_back(std::get<1>(segment_tuple));
                    }
                    if (std::get<2>(segment_tuple).get_length() > 0) {
                        segments.push_back(std::get<2>(segment_tuple));
                    }
                    segments[i] = segments[segments.size() - 1];
                    segments.pop_back();
                    segments[j] = segments[segments.size() - 1];
                    segments.pop_back();
                    operation = true;
                }
            }
        }
    }
    operation = true;
    while (operation && segments.size() > 0) {  // split intersected segments
        operation = false;
        for (size_t i = 0; i < segments.size() - 1; ++i) {
            for (size_t j = i + 1; j < segments.size(); ++j) {
                CoordPair intersection =
                    segments[i].get_instersect(segments[j]);
                if (intersection != CoordPair(-1, -1) &&
                    intersection != segments[i].get_start()) {
                    // std::cout << "i = " << i << " j = " << j << "
                    // intersection = "
                    //       << intersection.first << ", " <<
                    //       intersection.second << std::endl;
                    // std::cerr << segments[i] << " / " << segments[j] <<
                    // std::endl;
                    NetSegment splitted_1 =
                        segments[i].split_segment(intersection);
                    NetSegment splitted_2 =
                        segments[j].split_segment(intersection);
                    // std::cerr << segments[i] << " / " << segments[j] <<
                    // std::endl; std::cerr << splitted_1 << " / " << splitted_2
                    // << std::endl;
                    if (splitted_1.get_length() > 0)
                        segments.push_back(splitted_1);
                    if (splitted_2.get_length() > 0)
                        segments.push_back(splitted_2);
                    operation = true;
                }
            }
        }
    }
    // print_segments();

    // Construct graph by segments
    unsigned vNum = pNum;
    std::vector<int> used_pins(pNum);
    for (size_t i = 0; i < segments.size(); ++i) {  // Check which pins are used
        unsigned start_idx, end_idx;
        CoordPair start_coord = segments[i].get_start();
        CoordPair end_coord = segments[i].get_end();
        if (Coord2Vertices.contains(start_coord)) {
            start_idx = Coord2Vertices[start_coord];
            if (start_idx < pNum) {
                used_pins[start_idx] = 1;
            }
        }
        if (Coord2Vertices.contains(end_coord)) {
            end_idx = Coord2Vertices[end_coord];
            if (end_idx < pNum) {
                used_pins[end_idx] = 1;
            }
        }
    }
    for (size_t i = 0; i < pNum; ++i) {
        if (used_pins[i] == 0) {  // the pin must be on a segment
            // std::cerr << "Unused pin : " << Vertices[i].first << " " <<
            // Vertices[i].second << "\n";
            for (size_t j = 0; j < segments.size(); ++j) {
                if (segments[j].contains(Vertices[i])) {
                    NetSegment splitted =
                        segments[j].split_segment(Vertices[i]);
                    if (splitted.get_length() > 0)
                        segments.push_back(splitted);
                }
            }
        }
    }
    // std::cerr << segments.size() << "\n";
    for (size_t i = 0; i < segments.size(); ++i) {  // Add segments to graph
        unsigned start_idx, end_idx;
        CoordPair start_coord = segments[i].get_start();
        CoordPair end_coord = segments[i].get_end();
        if (Coord2Vertices.contains(start_coord)) {
            start_idx = Coord2Vertices[start_coord];
        } else {
            Vertices[vNum] = start_coord;
            VertexLayer[vNum] = segments[i].get_layer();
            Coord2Vertices[start_coord] = vNum;
            start_idx = vNum++;
            // std::cerr << "Pseudo pin " << vNum - 1 << " : " <<
            // start_coord.first << ", " << start_coord.second << "\n";
        }
        if (Coord2Vertices.contains(end_coord)) {
            end_idx = Coord2Vertices[end_coord];
        } else {
            Vertices[vNum] = end_coord;
            VertexLayer[vNum] = segments[i].get_layer();
            Coord2Vertices[end_coord] = vNum;
            end_idx = vNum++;
            // std::cerr << "Pseudo pin " << vNum - 1 << " : " <<
            // end_coord.first << ", " << end_coord.second << "\n";
        }
        assert(start_idx != end_idx);
        EdgeGraph.push_back(SimpleEdge(start_idx, end_idx,
                                       segments[i].get_length(),
                                       segments[i].get_layer()));
    }
    // std::cerr << EdgeGraph.size() << " " << vNum << std::endl;
    // Kruskal's MST algorithm O(ElogV + E)
    sort(EdgeGraph.begin(), EdgeGraph.end());  // sort by length of edges
    SimpleUnionFind union_find(vNum);
    safe::vector<int> selected_edges(EdgeGraph.size(), 0);
    safe::vector<VEPair>
        TreeGraph[vNum];  // adjacent list for storing MST (unweighted)
    for (size_t i = 0; i < EdgeGraph.size(); ++i) {
        unsigned v1 = EdgeGraph[i].get_v1(), v2 = EdgeGraph[i].get_v2();
        if (!union_find.same(v1, v2)) {
            // std::cerr << "-- Selected Edge : " << i << " ( " << v1 << ", " <<
            // v2 << " )\n";
            selected_edges[i] = 1;
            union_find.merge(v1, v2);
            TreeGraph[v1].push_back(VEPair(v2, i));
            TreeGraph[v2].push_back(VEPair(v1, i));
        } else {
            // std::cerr << "Not selected : " << i << std::endl;
            // std::cerr << Vertices[v1].first << ", " << Vertices[v1].second <<
            // " / "
            //           << Vertices[v2].first << ", " << Vertices[v2].second <<
            //           std::endl;
        }
    }
    if (!union_find.check_all_merged()) {
        unsigned idx = union_find.first_not_merged();
        std::cerr << "First not merged: " << idx << " : " << Vertices[idx].first
                  << ", " << Vertices[idx].second << std::endl;
        assert(false);
    }

    union_find.clear();
    // Remove redundant edges O(E)
    if (vNum >= 2) {
        // for(size_t i = 0; i < vNum; ++i){
        //     for(size_t j = 0; j < TreeGraph[i].size(); ++j){
        //         std::cerr << "v1 : " << i << ", v2 : " <<
        //         TreeGraph[i][j].first << "\n";
        //     }
        // }
        // std::cerr << "pNum " << pNum << "\n";
        dfs_tree_graph(TreeGraph, selected_edges, pNum, 0, -1, -1);
    }
    // Reconstruct pruned tree O(E)
    for (size_t i = 0; i < vNum; ++i)
        TreeGraph[i].clear();
    safe::vector<SimpleEdge> SimpleTree[vNum];
    unsigned tree_size = 1;  // tree size = # of vertices = # of edges + 1
    for (size_t i = 0; i < EdgeGraph.size(); ++i) {
        // std::cout << selected_edges[i] << std::endl;
        if (selected_edges[i]) {
            // std::cerr << "Selected Edge : " << i << " ( " <<
            // EdgeGraph[i].get_v1() << ", " << EdgeGraph[i].get_v2() << " )\n";
            unsigned v1 = EdgeGraph[i].get_v1(), v2 = EdgeGraph[i].get_v2(),
                     layer = EdgeGraph[i].get_layer();
            SimpleTree[v1].push_back(SimpleEdge(v2, DINF, 0, layer));
            SimpleTree[v2].push_back(SimpleEdge(v1, DINF, 0, layer));
            ++tree_size;
        }
    }

    // Find the center of the tree
    safe::vector<double> vertex_rank(vNum, -1.0);
    dfs_tree_center(SimpleTree, vertex_rank, tree_size, 0, 0);
    unsigned found_center = 0;
    double min_score = DINF + 1.0;
    for (size_t i = 0; i < vNum; ++i) {
        if (vertex_rank[i] > -EPS && vertex_rank[i] < min_score) {
            min_score = vertex_rank[i];
            found_center = i;
        }
    }

    // Translate vertex indices to new indices
    safe::vector<int> new_idx_mapping(vNum);  // maps old indices to new ones
    unsigned new_idx = 0;
    for (size_t i = 0; i < vNum; ++i) {
        // std::cout << vertex_rank[i] << std::endl;
        if (vertex_rank[i] > -EPS) {
            new_idx_mapping[i] = new_idx++;
        } else {
            new_idx_mapping[i] = -1;
        }
    }
    // std::cout << new_idx << " " << tree_size << std::endl;
    assert(new_idx == tree_size);

    // Construct quad nodes
    for (size_t i = 0; i < new_idx; ++i) {
        nodes.push_back(QuadNode(i));
    }
    // Construct quad tree
    dfs_construct_tree(SimpleTree, Vertices, VertexLayer, new_idx_mapping,
                       found_center, -1);

    // Final preperation
    root_idx = new_idx_mapping[found_center];
    // assert(nodes[root_idx].get_parent() == -1);
    for (size_t i = 0; i < nodes.size();
         ++i) {  // set coordinate to node index mapping
        coord2Node[nodes[i].get_coord()] = i;
    }
    // std::cout << "tree size: " << nodes.size() << ", num pins: " <<
    // pins.size() << std::endl;

    segments.clear();
    vias.clear();
}

inline bool QuadTree::dfs_tree_graph(safe::vector<VEPair> TreeGraph[],
                                     safe::vector<int>& selected_edges,
                                     const unsigned pNum,
                                     const unsigned now,
                                     const int parent,
                                     const int edge_idx) {
    // Use DFS to remove redundant edges O(E)
    // If the end of a path is not a pin then some edges of this path are
    // redundant. std::cerr << "dfs -- now : " << now << ", parent : " << parent
    // << ", edge_idx : " << edge_idx << "\n";
    bool has_pin = false;
    for (size_t i = 0; i < TreeGraph[now].size(); ++i) {
        if ((int)TreeGraph[now][i].first == parent)
            continue;
        // std::cerr << "now : " << now << ", child : " <<
        // TreeGraph[now][i].first << ", parent " << parent << "\n";
        has_pin |= dfs_tree_graph(TreeGraph, selected_edges, pNum,
                                  TreeGraph[now][i].first, now,
                                  TreeGraph[now][i].second);
    }
    has_pin |= now < pNum;
    // std::cerr << now << " " << parent << " children : " <<
    // TreeGraph[now].size() << " edge : " << edge_idx << " has pin" << has_pin
    // << "\n";
    if (has_pin)
        return true;
    else if (edge_idx >= 0) {
        selected_edges[edge_idx] = 0;
        return false;
    }
    return false;
}

inline unsigned QuadTree::dfs_tree_center(safe::vector<SimpleEdge> SimpleTree[],
                                          safe::vector<double>& vertex_rank,
                                          const unsigned tree_size,
                                          const unsigned now,
                                          const unsigned parent) {
    // Use DFS to find the center of the tree O(E)
    safe::vector<unsigned> num_vertices;
    for (size_t i = 0; i < SimpleTree[now].size(); ++i) {
        if (SimpleTree[now][i].get_v1() != parent) {
            unsigned num_branch_vertices =
                dfs_tree_center(SimpleTree, vertex_rank, tree_size,
                                SimpleTree[now][i].get_v1(), now);
            if (num_branch_vertices == 0)
                continue;
            num_vertices.push_back(num_branch_vertices);
        }
    }
    size_t num_branch = 0;
    if (now != parent) {  // first DFS node will be now = parent = 0
        num_branch = 1 + num_vertices.size();
    } else {
        num_branch = num_vertices.size();
    }
    if (num_branch ==
        0) {  // only happens when there is only one node in the tree
        vertex_rank[now] = 0.0;
        return 0;
    }
    if (num_branch == 1) {  // leaf node
        vertex_rank[now] = DINF;
        return 1;
    }
    double score = 0.0;
    unsigned total = 0;
    double average_score = ((double)tree_size - 1.0) / (double)num_branch;
    for (size_t i = 0; i < num_vertices.size(); ++i) {
        total += num_vertices[i];
        score += ((double)num_vertices[i] - average_score) *
                 ((double)num_vertices[i] - average_score);
    }
    score += ((double)(tree_size - total - 1) - average_score) *
             ((double)(tree_size - total - 1) - average_score);
    score /= (double)num_branch;  // == variance
    vertex_rank[now] = score;
    return total + 1;
}

inline void QuadTree::dfs_construct_tree(
    safe::vector<SimpleEdge> SimpleTree[],
    safe::unordered_map<unsigned, CoordPair>& Vertices,
    safe::unordered_map<unsigned, int>& VertexLayer,
    safe::vector<int>& new_idx_mapping,
    const unsigned now,
    const int parent) {
    // Use DFS to construct quad tree
    // nodes[new_idx_mapping[now]].set_parent(parent);
    nodes[new_idx_mapping[now]].set_layer_self(
        VertexLayer[now]);  // TODO: not sure if it makes sense
    nodes[new_idx_mapping[now]].reset_coord(Vertices[now]);
    for (size_t i = 0; i < SimpleTree[now].size(); ++i) {
        unsigned direction = check_direction(
            Vertices[now], Vertices[SimpleTree[now][i].get_v1()]);
        if (direction == 1) {  // up
            nodes[new_idx_mapping[now]].set_up(
                new_idx_mapping[SimpleTree[now][i].get_v1()]);
            nodes[new_idx_mapping[now]].set_layer_up(
                SimpleTree[now][i].get_layer());
        } else if (direction == 2) {  // down
            nodes[new_idx_mapping[now]].set_down(
                new_idx_mapping[SimpleTree[now][i].get_v1()]);
            nodes[new_idx_mapping[now]].set_layer_down(
                SimpleTree[now][i].get_layer());
        } else if (direction == 3) {  // left
            nodes[new_idx_mapping[now]].set_left(
                new_idx_mapping[SimpleTree[now][i].get_v1()]);
            nodes[new_idx_mapping[now]].set_layer_left(
                SimpleTree[now][i].get_layer());
        } else if (direction == 4) {  // right
            nodes[new_idx_mapping[now]].set_right(
                new_idx_mapping[SimpleTree[now][i].get_v1()]);
            nodes[new_idx_mapping[now]].set_layer_right(
                SimpleTree[now][i].get_layer());
        }
        if ((int)SimpleTree[now][i].get_v1() != parent) {
            dfs_construct_tree(SimpleTree, Vertices, VertexLayer,
                               new_idx_mapping, SimpleTree[now][i].get_v1(),
                               now);
        }
    }
}

unsigned QuadTree::check_direction(const CoordPair c_1,
                                   const CoordPair c_2) const {
    // find the relative direction between the two vertices
    if (c_1.first < c_2.first && c_1.second == c_2.second)
        return 1;  // up
    if (c_1.first > c_2.first && c_1.second == c_2.second)
        return 2;  // down
    if (c_1.first == c_2.first && c_1.second < c_2.second)
        return 3;  // left
    if (c_1.first == c_2.first && c_1.second > c_2.second)
        return 4;  // right
    assert(false);
    return 0;
}

void QuadTree::tree_to_segment() {
    segments.clear();
    dfs_extract_segments(0, -1);
    // Fix problems of different pins with same coordinates but different
    // layers. Add via for those pins
    for (size_t i = 0; i < pins.size(); ++i) {
        // std::cerr << nodes[pins2NodeIdx[i]].get_layer_self() << " " <<
        // pins[i].get_layer() << "\n";
        size_t idx = pins[i].get_idx();
        if (nodes[pinIdx2Node[idx]].get_layer_self() !=
            (int)pins[i].get_layer()) {
            segments.push_back(NetSegment(
                nodes[pinIdx2Node[idx]].get_coord_x(),
                nodes[pinIdx2Node[idx]].get_coord_y(),
                nodes[pinIdx2Node[idx]].get_coord_x(),
                nodes[pinIdx2Node[idx]].get_coord_y(),
                nodes[pinIdx2Node[idx]].get_layer_self(), pins[i].get_layer()));
        }
    }
    if (segments.size() == 0) {
        // TODO: choose the pin closest to the minLayer
        size_t closest = 0, closest_idx = 0;
        int min_dist = DINF;
        // std::cerr << "minLayer : " << _minLayer << "\n";
        for (size_t i = 0; i < pins.size(); ++i) {
            // std::cerr << nodes[pins2NodeIdx[i]].get_layer_self() << " " <<
            // pins[i].get_layer() << "\n";
            int dist = _minLayer - (int)pins[i].get_layer();
            if (dist >= 0 && dist < min_dist) {
                closest = pins[i].get_idx();
                closest_idx = i;
                min_dist = dist;
            }
        }
        if (min_dist > 0 && min_dist < DINF) {
            segments.push_back(
                NetSegment(nodes[pinIdx2Node[closest]].get_coord_x(),
                           nodes[pinIdx2Node[closest]].get_coord_y(),
                           nodes[pinIdx2Node[closest]].get_coord_x(),
                           nodes[pinIdx2Node[closest]].get_coord_y(),
                           pins[closest_idx].get_layer(), _minLayer));
        }
    }
}

inline void QuadTree::dfs_extract_segments(const unsigned now,
                                           const int parent) {
    if (nodes[now].has_up() && nodes[now].get_up() != parent) {
        int up = nodes[now].get_up();
        segments.push_back(
            NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                       nodes[up].get_coord_x(), nodes[up].get_coord_y(),
                       nodes[now].get_layer_up()));
        if (nodes[now].get_layer_self() != nodes[now].get_layer_up()) {
            segments.push_back(
                NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_layer_self(),
                           nodes[now].get_layer_up()));  // via
        }
        if (nodes[now].get_layer_up() != nodes[up].get_layer_self()) {
            segments.push_back(NetSegment(
                nodes[up].get_coord_x(), nodes[up].get_coord_y(),
                nodes[up].get_coord_x(), nodes[up].get_coord_y(),
                nodes[now].get_layer_up(), nodes[up].get_layer_self()));  // via
        }
        dfs_extract_segments(up, now);
    }
    if (nodes[now].has_down() && nodes[now].get_down() != parent) {
        int down = nodes[now].get_down();
        segments.push_back(
            NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                       nodes[down].get_coord_x(), nodes[down].get_coord_y(),
                       nodes[now].get_layer_down()));
        if (nodes[now].get_layer_self() != nodes[now].get_layer_down()) {
            segments.push_back(
                NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_layer_self(),
                           nodes[now].get_layer_down()));  // via
        }
        if (nodes[now].get_layer_down() != nodes[down].get_layer_self()) {
            segments.push_back(
                NetSegment(nodes[down].get_coord_x(), nodes[down].get_coord_y(),
                           nodes[down].get_coord_x(), nodes[down].get_coord_y(),
                           nodes[now].get_layer_down(),
                           nodes[down].get_layer_self()));  // via
        }
        dfs_extract_segments(down, now);
    }
    if (nodes[now].has_left() && nodes[now].get_left() != parent) {
        int left = nodes[now].get_left();
        segments.push_back(
            NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                       nodes[left].get_coord_x(), nodes[left].get_coord_y(),
                       nodes[now].get_layer_left()));
        if (nodes[now].get_layer_self() != nodes[now].get_layer_left()) {
            segments.push_back(
                NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_layer_self(),
                           nodes[now].get_layer_left()));  // via
        }
        if (nodes[now].get_layer_left() != nodes[left].get_layer_self()) {
            segments.push_back(
                NetSegment(nodes[left].get_coord_x(), nodes[left].get_coord_y(),
                           nodes[left].get_coord_x(), nodes[left].get_coord_y(),
                           nodes[now].get_layer_left(),
                           nodes[left].get_layer_self()));  // via
        }
        dfs_extract_segments(left, now);
    }
    if (nodes[now].has_right() && nodes[now].get_right() != parent) {
        int right = nodes[now].get_right();
        segments.push_back(
            NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                       nodes[right].get_coord_x(), nodes[right].get_coord_y(),
                       nodes[now].get_layer_right()));
        if (nodes[now].get_layer_self() != nodes[now].get_layer_right()) {
            segments.push_back(
                NetSegment(nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_coord_x(), nodes[now].get_coord_y(),
                           nodes[now].get_layer_self(),
                           nodes[now].get_layer_right()));  // via
        }
        if (nodes[now].get_layer_right() != nodes[right].get_layer_self()) {
            segments.push_back(NetSegment(
                nodes[right].get_coord_x(), nodes[right].get_coord_y(),
                nodes[right].get_coord_x(), nodes[right].get_coord_y(),
                nodes[now].get_layer_right(),
                nodes[right].get_layer_self()));  // via
        }
        dfs_extract_segments(right, now);
    }
}

void QuadTree::print_segments() {
    for (size_t i = 0; i < segments.size(); ++i) {
        std::cout << "i = " << i << ", " << segments[i] << std::endl;
    }
}

std::ostream& operator<<(std::ostream& out, const QuadTree& qt) {
    // out << "NumRoutes" << qt.segments.size() << std::endl;
    for (size_t i = 0; i < qt.segments.size(); ++i) {
        out << qt.segments[i].get_xs() + qt._baseRow << " "
            << qt.segments[i].get_ys() + qt._baseCol << " "
            << qt.segments[i].get_layer() + 1 << " "
            << qt.segments[i].get_xe() + qt._baseRow << " "
            << qt.segments[i].get_ye() + qt._baseCol << " "
            << qt.segments[i].get_layer_end() + 1 << " "
            << "N" << qt._NetId + 1 << "\n";
    }
    return out;
}

// ! migrated from QuadExt.cpp
typedef safe::vector<SimplePin>::const_iterator citer;

enum class CmpType { Minimum, Maximum };
enum class UseArg { Yes, No };

// if arg then return arg(min,max) else return (min,max)
template <CmpType t, UseArg arg>
static unsigned get_(const safe::vector<SimplePin>& pins,
                     std::function<unsigned(citer)> get) {
    auto iter = pins.begin();
    unsigned val = get(iter);
    unsigned idx = 0, idx_val = 0;
    for (++iter, ++idx; iter != pins.end(); ++iter, ++idx) {
        unsigned cur = get(iter);
        switch (t) {
            case CmpType::Minimum:
                if (cur < val) {
                    val = cur;
                    idx_val = idx;
                }
                break;
            case CmpType::Maximum:
                if (cur > val) {
                    val = cur;
                    idx_val = idx;
                }
                break;
        }
    }
    switch (arg) {
        case UseArg::Yes:
            return idx_val;
        case UseArg::No:
            return val;
    }
}

template <UseArg arg>
static unsigned left(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_row(); };
    return get_<CmpType::Minimum, arg>(pins, func);
}

template <UseArg arg>
static unsigned right(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_row(); };
    return get_<CmpType::Maximum, arg>(pins, func);
}

template <UseArg arg>
static unsigned bottom(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_col(); };
    return get_<CmpType::Minimum, arg>(pins, func);
}

template <UseArg arg>
static unsigned top(const safe::vector<SimplePin>& pins) {
    auto func = [=](citer c) { return c->get_col(); };
    return get_<CmpType::Maximum, arg>(pins, func);
}

unsigned QuadTree::get_left(void) const {
    return left<UseArg::No>(pins);
}

unsigned QuadTree::get_right(void) const {
    return right<UseArg::No>(pins);
}

unsigned QuadTree::get_bottom(void) const {
    return bottom<UseArg::No>(pins);
}

unsigned QuadTree::get_top(void) const {
    return top<UseArg::No>(pins);
}

unsigned QuadTree::get_left_pin(void) const {
    unsigned p = left<UseArg::Yes>(pins);
    return pins[p].get_idx();
}

unsigned QuadTree::get_right_pin(void) const {
    unsigned p = right<UseArg::Yes>(pins);
    return pins[p].get_idx();
}

unsigned QuadTree::get_bottom_pin(void) const {
    unsigned p = bottom<UseArg::Yes>(pins);
    return pins[p].get_idx();
}

unsigned QuadTree::get_top_pin(void) const {
    unsigned p = top<UseArg::Yes>(pins);
    return pins[p].get_idx();
}

std::pair<unsigned, unsigned> QuadTree::get_horiz_bound(void) const {
    unsigned left = get_left(), right = get_right();
    assert(left <= right);
    return std::make_pair(left, right);
}

std::pair<unsigned, unsigned> QuadTree::get_verti_bound(void) const {
    unsigned bottom = get_bottom(), top = get_top();
    assert(bottom <= top);
    return std::make_pair(bottom, top);
}

unsigned QuadTree::get_hpwl(void) const {
    auto horiz = get_horiz_bound(), verti = get_verti_bound();
    return horiz.second - horiz.first + verti.second - verti.first;
}
