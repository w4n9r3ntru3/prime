// * Copyright (C) Heng-Jui Chang, Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <utility>

#include "Chip.h"
#include "QuadNode.h"
#include "safe.h"

// vertex index v.s. edge index pair
typedef std::pair<unsigned, unsigned> VEPair;
const double DINF = 1000000000;
const double EPS = 1e-8;
const unsigned DEFAULT_OPT = 1;
class NetSegment;

class QuadTree {
   public:
    // constructor
    QuadTree() noexcept;
    QuadTree(int n_id,
             int min_lay,
             int base_row,
             int base_col,
             int max_row,
             int max_col,
             int max_layer) noexcept;
    // ~QuadTree() noexcept;

    // access to basic attributes
    // std::string    get_name() const;
    int get_net_id() const;
    int get_min_layer() const;
    int get_root_idx() const;
    bool is_built() const;

    // get nodes / pins
    unsigned size() const;
    unsigned pin_num() const;
    unsigned pseudo_pin_num() const;
    bool exist_node(const CoordPair& _coord) const;
    bool exist_node(const unsigned _x, const unsigned _y) const;
    bool is_pin(unsigned idx) const;
    bool is_pseudo_pin(unsigned idx) const;
    int get_node_idx(const CoordPair& _coord) const;
    int get_node_idx(const unsigned _x, const unsigned _y) const;
    QuadNode& get_node(unsigned idx);
    QuadNode& get_node(const CoordPair& _coord);
    QuadNode& get_node(const unsigned _x, const unsigned _y);
    safe::vector<SimplePin>& get_pin_list();
    safe::vector<NetSegment>& get_segments();

    // get information about the net
    unsigned get_net_length() const;
    unsigned get_subnet_length(unsigned idx) const;
    CoordPair get_bounding_box_size() const;

    // operations on the pins / pseudo-pins
    int move_vertical(unsigned idx, int delta_x);
    int move_horizontal(unsigned idx, int delta_y);

    // operations on the whole net
    // void optimize(unsigned max_iter = DEFAULT_OPT);

    // constructing the tree
    void add_pin(SimplePin p);  // TODO: is this necessary?
    void add_segment(int srow,
                     int scol,
                     int slay,
                     int erow,
                     int ecol,
                     int elay);
    void construct_tree();
    void convert_to_segments();
    void reset_tree();

    unsigned get_left(void) const;
    unsigned get_right(void) const;
    unsigned get_bottom(void) const;
    unsigned get_top(void) const;
    unsigned get_left_pin(void) const;
    unsigned get_right_pin(void) const;
    unsigned get_bottom_pin(void) const;
    unsigned get_top_pin(void) const;
    std::pair<unsigned, unsigned> get_horiz_bound(void) const;
    std::pair<unsigned, unsigned> get_verti_bound(void) const;
    unsigned get_hpwl(void) const;

    // TODO: use union find to create a tree

   private:
    // const std::string                _NetName;
    const int _NetId;
    const int _baseRow;
    const int _baseCol;
    const int _minLayer;
    const int _maxRows;
    const int _maxCols;
    const int _maxLayers;

    int root_idx;
    unsigned flag;
    safe::vector<QuadNode> nodes;  // pins will be at the front of this vector
    safe::map<CoordPair, unsigned> coord2Node;
    safe::vector<SimplePin> pins;  // index of pins
    // mapping from pins to index of nodes
    safe::unordered_map<unsigned, unsigned> pinIdx2Node;
    // TODO: nodeIdx2Pin
    // Temporary members for constructing the tree
    safe::vector<NetSegment> segments;
    safe::vector<NetSegment> vias;  // FIXME:

    // Private functions
    // Basic operations
    // FIXME:
    void increment_flag();
    bool can_merge(QuadNode& n_1, QuadNode& n_2, const std::string dir) const;
    void merge_nodes(QuadNode& n_1, QuadNode& n_2, const std::string dir);
    void insert_node();
    void delete_node();
    inline int move_pin(unsigned idx, int delta_x, int delta_y);

    // Optimization
    // void self_optimize();

    // Segment / Tree conversion functions
    // Don't move :
    void segment_to_tree();
    inline bool dfs_tree_graph(safe::vector<VEPair> TreeGraph[],
                               safe::vector<int>& selected_edges,
                               const unsigned pNum,
                               const unsigned now,
                               const int parent,
                               const int edge_idx);
    inline unsigned dfs_tree_center(safe::vector<SimpleEdge> SimpleTree[],
                                    safe::vector<double>& vertex_rank,
                                    const unsigned tree_size,
                                    const unsigned now,
                                    const unsigned parent);
    inline void dfs_construct_tree(
        safe::vector<SimpleEdge> SimpleTree[],
        safe::unordered_map<unsigned, CoordPair>& Vertices,
        safe::unordered_map<unsigned, int>& VertexLayer,
        safe::vector<int>& new_idx_mapping,
        const unsigned now,
        const int parent);
    unsigned check_direction(const CoordPair c_1, const CoordPair c_2) const;

    void tree_to_segment();
    inline void dfs_extract_segments(const unsigned now, const int parent);

    // Debug functions
    void print_segments();

    // friends
    // TODO: print tree
    friend std::ostream& operator<<(std::ostream& out, const QuadTree& qt);
};
