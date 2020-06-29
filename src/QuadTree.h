// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>
#include <assert.h>
#include <utility>
#include <algorithm>

#include "QuadNode.h"
#include "QuadUtil.h"
#include "Cell.h"
#include "safe.h"

typedef std::pair<unsigned, unsigned> VEPair; // vertex index v.s. edge index pair
const double DINF = 1e9;
const double EPS  = 1e-8;
const unsigned DEFAULT_OPT = 1;
class NetSegment;

class QuadTree {
   public:
    // constructor
    QuadTree() noexcept;
    QuadTree(std::string n, 
        int n_id, int min_lay, 
        int max_row, int max_col) noexcept;
    ~QuadTree() noexcept;

    // access to basic attributes
    const std::string&    get_name() const;
    const int&          get_net_id() const;
    const int&       get_min_layer() const;
    const int&        get_root_idx() const;
    bool                  is_built() const;

    // get nodes / pins
    unsigned  size() const;
    unsigned  pin_num() const;
    unsigned  pseudo_pin_num() const;
    bool      exist_node(const CoordPair& _coord) const;
    bool      exist_node(const unsigned _x, const unsigned _y) const;
    bool      is_pin(unsigned idx) const;
    bool      is_pseudo_pin(unsigned idx) const;
    int       get_node_idx(const CoordPair& _coord) const;
    int       get_node_idx(const unsigned _x, const unsigned _y) const;
    QuadNode& get_node(unsigned idx);
    QuadNode& get_node(const CoordPair& _coord);
    QuadNode& get_node(const unsigned _x, const unsigned _y);

    // get information about the net
    unsigned get_net_length() const;
    unsigned get_subnet_length(unsigned idx) const;
    CoordPair get_bounding_box_size() const;

    // operations on the pins / pseudo-pins
    int move_vertical(unsigned idx, int delta_x);
    int move_horizontal(unsigned idx, int delta_y);

    // operations on the whole net
    void optimize(unsigned max_iter = DEFAULT_OPT);

    // constructing the tree
    void add_pin(Pin* p);
    void add_segment(int srow, int scol, int slay, int erow, int ecol, int elay);
    void construct_tree();
    void reset_tree();

   private:
    const std::string                _NetName;
    const int                          _NetId;
    const int                       _minLayer;
    const int                        _maxRows;
    const int                        _maxCols;
    
    int                              root_idx;
    unsigned                             flag;
    safe::vector<QuadNode>              nodes; // pins will be at the front of this vector
    safe::map<CoordPair, unsigned> coord2Node;
    safe::vector<Pin*>                   pins;
    
    // Temporary members for constructing the tree
    safe::vector<NetSegment>         segments;

    // Private functions
    // Basic operations
    void increment_flag();
    bool can_merge(QuadNode& n_1, QuadNode& n_2, const std::string dir) const;
    void merge_nodes(QuadNode& n_1, QuadNode& n_2, const std::string dir);
    void insert_node();
    void delete_node();
    inline int move_pin(unsigned idx, int delta_x, int delta_y);
    

    // Optimization
    void self_optimize();

    // Segment / Tree conversion functions
    void segment_to_tree();
    inline bool dfs_tree_graph(safe::vector<VEPair> TreeGraph[], 
                               safe::vector<int>&  selected_edges, 
                               const unsigned vNum, const unsigned pNum, 
                               const unsigned now, const unsigned parent,
                               const unsigned edge_idx);
    inline unsigned dfs_tree_center(safe::vector<unsigned> SimpleTree[],
                                    safe::vector<double>&  vertex_rank, 
                                    const unsigned tree_size,
                                    const unsigned now, const unsigned parent);
    inline void dfs_construct_tree(safe::vector<unsigned> SimpleTree[],
                                   safe::unordered_map<unsigned, CoordPair>& Vertices,
                                   safe::vector<int>& new_idx_mapping,
                                   const unsigned now, const int parent);
    unsigned check_direction(const CoordPair c_1, const CoordPair c_2) const;
    
    void tree_to_segment(); // TODO: return net as segments

    
    // friends
    // TODO: print tree
};
