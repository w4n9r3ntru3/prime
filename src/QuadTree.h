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
#include "Cell.h"
#include "safe.h"

typedef std::pair<unsigned, unsigned> VEPair; // vertex index v.s. edge index pair
const double DINF = 1e9;
const double EPS  = 1e-8;
class NetSegment;

class QuadTree {
   public:
    // constructor
    QuadTree() noexcept;
    explicit QuadTree(std::string n, int n_id, int min_lay) noexcept;
    ~QuadTree() noexcept;

    const std::string&    get_name() const;
    const int           get_net_id() const;
    const int        get_min_layer() const;
    const int         get_root_idx() const;

    void add_pin(Pin* p);
    void add_segment(int srow, int scol, int slay, int erow, int ecol, int elay);
    void construct_tree();

   private:
    const std::string       _NetName;
    const int                 _NetId;
    const int              _minLayer;
    
    int                     root_idx;
    safe::vector<QuadNode>     nodes; // pins will be at the front of this vector
    safe::vector<Pin*>          pins;
    safe::unordered_map<CoordPair, unsigned> pinCoord2Idx;

    // Temporary members for constructing the tree
    safe::vector<NetSegment> segments;

    // Private functions
    void insert_node();

    void segment_to_tree();
    inline bool dfs_tree_graph(safe::vector<VEPair> TreeGraph[], 
                               safe::vector<bool>&  selected_edges, 
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
    const unsigned check_direction(const CoordPair c_1, const CoordPair c_2) const;
    void tree_to_segment();

    // friends
    // TODO: return net as segments
};

// NetSegment: the class for storing the segments of nets from the input
class NetSegment{
   public:
    // constructor
    NetSegment() noexcept
        : x_start(-1), y_start(-1), x_end(-1), y_end(-1) {}
    NetSegment(int xs, int ys, int xe, int ye) noexcept
        : x_start(xs), y_start(ys), x_end(xe), y_end(ye){
            assert(x_start <= x_end);
            assert(y_start <= y_end);
        }
    
    NetSegment& operator=(const NetSegment& ns) noexcept {
        x_start = ns.x_start; x_end = ns.x_end;
        y_start = ns.y_start; y_end = ns.y_end;
    }
    const bool operator<(const NetSegment& ns) const {
        if      (x_start < ns.get_xs()) return  true;
        else if (x_start > ns.get_xs()) return false;
        else if (x_end   < ns.get_xe()) return  true;
        else if (x_end   > ns.get_xe()) return false;
        else if (y_start < ns.get_ys()) return  true;
        else if (y_start > ns.get_ys()) return false;
        else if (y_end   < ns.get_ye()) return  true;
        else if (y_end   > ns.get_ye()) return false;
        else return true;
    }

    const int get_xs() const { return x_start; }
    const int get_ys() const { return y_start; }
    const int get_xe() const { return   x_end; }
    const int get_ye() const { return   y_end; }
    const CoordPair get_start() const { return CoordPair(x_start, y_start); }
    const CoordPair get_end()   const { return CoordPair(x_end,   y_end);   }
    // direction: true -> vertical, false -> horizontal
    const bool get_direction() const { return (x_start < x_end) ? true : false; }
    const unsigned get_length() const { return (x_end - x_start) + (y_end - y_start); }

    const bool check_overlap(const NetSegment& ns) const {
        // Check whether two parallel segments overlap
        if(get_direction() != ns.get_direction()) return false;
        if(get_direction() && y_start == ns.get_ys()){        // vertical
            if(x_start <= ns.get_xs() && x_end   >= ns.get_xs()) return true;
            if(x_start >= ns.get_xs() && x_start <= ns.get_xe()) return true;
        }
        else if(!get_direction() && x_start == ns.get_xs()) { // horizontal
            if(y_start <= ns.get_ys() && y_end   >= ns.get_ys()) return true;
            if(y_start >= ns.get_ys() && y_start <= ns.get_ye()) return true;
        }
        return false;
    }
    const CoordPair check_shared_point(const NetSegment& ns) const {
        if(x_start == ns.get_xs() && y_start == ns.get_ys()) return CoordPair(x_start, y_start);
        if(x_start == ns.get_xe() && y_start == ns.get_ye()) return CoordPair(x_start, y_start);
        if(x_end   == ns.get_xs() && y_end   == ns.get_ys()) return CoordPair(  x_end,   y_end);
        if(x_end   == ns.get_xe() && y_end   == ns.get_ye()) return CoordPair(  x_end,   y_end);
        return CoordPair(-1, -1);
    }
    const CoordPair get_instersect(const NetSegment& ns) const {
        // Find the intersection of the two orthogonal segments
        if(get_direction() == ns.get_direction()) return CoordPair(-1, -1);
        if(check_shared_point(ns) != CoordPair(-1, -1)) return CoordPair(-1, -1);
        if(get_direction()){ // vertical
            if(x_start <= ns.get_xs() && ns.get_xs() <= x_end
                && ns.get_ys() <= y_start && y_start <= ns.get_ys()){
                    return CoordPair(ns.get_xs(), y_start);
            }
        } else {             // horizontal
            if(y_start <= ns.get_ys() && ns.get_ys() <= y_end
                && ns.get_xs() <= x_start && x_start <= ns.get_xs()){
                    return CoordPair(x_start, ns.get_ys());
            }
        }
        return CoordPair(-1, -1);
    }
    const bool check_instersect(const NetSegment& ns) const { return get_instersect(ns) != CoordPair(-1, -1); }

    void merge_segment(NetSegment& ns) { // merge two parallel segments
        if(!check_overlap(ns)) return;
        if(get_direction()){ // vertical
            if (x_start > ns.get_xs()) x_start = ns.get_xs();
            if (x_end   < ns.get_xe()) x_end   = ns.get_xe();
        } else {             // horizontal
            if (y_start > ns.get_ys()) x_start = ns.get_ys();
            if (y_end   < ns.get_ye()) x_end   = ns.get_ye();
        }
    }
    NetSegment& split_segment(CoordPair& coord) { // split segment
        if(get_direction()){ // vertical (difference: x)
            NetSegment splitted(coord.first, y_start, x_end, y_end);
            x_end = coord.first;
            return splitted;
        } else { // horizontal
            NetSegment splitted(x_start, coord.second, x_end, y_end);
            y_end = coord.second;
            return splitted;
        }
    }

   private:
    int x_start, x_end;
    int y_start, y_end;
};

// SimpleUnionFind: a simple union find class for Kruskal's MST algorithm
class SimpleUnionFind{
   public:
    SimpleUnionFind() noexcept {};
    SimpleUnionFind(int N) noexcept { reset(N); }
    ~SimpleUnionFind() noexcept { clear(); }

    inline unsigned find(unsigned x) {
        return (x == parent[x]) ? x : parent[x] = find(parent[x]);
    }
    inline bool same(unsigned x, unsigned y) {
        return find(x) == find(y);
    }
    inline void merge(unsigned x, unsigned y) {
        x = find(x);
        y = find(y);
        if(x == y) return;
        if(rank[x] < rank[y]){
            parent[x] = y;
        } else {
            if(rank[x] == rank[y]) ++rank[x];
            parent[y] = x;
        }
    }
    void clear() { parent.clear(); rank.clear(); }
    void reset(int N){
        clear();
        parent.resize(N);
        rank.resize(N);
        for(int i = 0; i < N; ++i) parent[i] = i;
    } 
    
   private:
    safe::vector<unsigned> parent;
    safe::vector<unsigned> rank;
};