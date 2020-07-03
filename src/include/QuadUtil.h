// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>
#include <assert.h>
#include <utility>
#include <algorithm>

#include "Cell.h"
#include "safe.h"

typedef std::pair<int, int> CoordPair; // Coordinate of points

// NetSegment: the class for storing the segments of nets from the input
class NetSegment{
   public:
    // constructor
    NetSegment() noexcept;
    NetSegment(int xs, int ys, int xe, int ye, int l) noexcept;
    NetSegment(int xs, int ys, int xe, int ye, int l, int le) noexcept;

    // NetSegment& operator=(const NetSegment& ns) noexcept {
    //     x_start = ns.get_xs(); x_end = ns.get_xe();
    //     y_start = ns.get_ys(); y_end = ns.get_ye();
    //     layer   = ns.get_layer();
    //     return *this;
    // }
    // NetSegment& operator=(NetSegment&& ns) noexcept {
    //     x_start = std::move(ns.x_start); x_end = std::move(ns.x_end);
    //     y_start = std::move(ns.y_start); y_end = std::move(ns.y_end);
    //     layer   = std::move(ns.layer);
    //     return *this;
    // }
    bool operator<(const NetSegment& ns) const;
    bool operator!=(const NetSegment& ns) const;

    const int& get_xs() const;
    const int& get_ys() const;
    const int& get_xe() const;
    const int& get_ye() const;
    const int& get_layer() const;
    const int& get_layer_end() const;
    CoordPair get_start() const;
    CoordPair get_end() const;
    // direction: true -> vertical, false -> horizontal
    bool get_direction() const;
    unsigned get_length() const;

    bool check_overlap(const NetSegment& ns) const;
    CoordPair check_shared_point(const NetSegment& ns) const;
    CoordPair get_instersect(const NetSegment& ns) const;
    bool check_instersect(const NetSegment& ns) const;

    void merge_segment(NetSegment& ns);
    NetSegment split_segment(CoordPair& coord);

   private:
    int x_start, y_start, x_end, y_end, layer_start, layer_end;

    friend std::ostream& operator<<(std::ostream& out, const NetSegment& ns);
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

// Utility functions
int clamp(int _x, int _min, int _max);
int ABS(int _x);
int MIN(int _x, int _y);
int MAX(int _x, int _y);
unsigned dir2Num(const std::string s);
std::string num2Dir(const unsigned dir);
unsigned opposite_dir(unsigned dir);
unsigned opposite_dir(const std::string s);

