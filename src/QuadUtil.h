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

// NetSegment: the class for storing the segments of nets from the input
class NetSegment{
   public:
    // constructor
    NetSegment() noexcept
        : x_start(-1), y_start(-1), x_end(-1), y_end(-1) {}
    NetSegment(int xs, int ys, int xe, int ye, int l) noexcept
        : x_start(xs), y_start(ys), x_end(xe), y_end(ye), layer(l) {
            assert(x_start <= x_end);
            assert(y_start <= y_end);
        }
    
    NetSegment& operator=(const NetSegment& ns) noexcept {
        x_start = ns.x_start; x_end = ns.x_end;
        y_start = ns.y_start; y_end = ns.y_end;
        return *this;
    }
    bool operator<(const NetSegment& ns) const {
        if      (x_start < ns.get_xs()) return  true;
        else if (x_start > ns.get_xs()) return false;
        else if (x_end   < ns.get_xe()) return  true;
        else if (x_end   > ns.get_xe()) return false;
        else if (y_start < ns.get_ys()) return  true;
        else if (y_start > ns.get_ys()) return false;
        else if (y_end   < ns.get_ye()) return  true;
        else if (y_end   > ns.get_ye()) return false;
        else if (layer   < ns.get_layer()) return true;
        else return true;
    }
    bool operator!=(const NetSegment& ns) const {
        return x_start != ns.get_xs() || y_start != ns.get_ys() || x_end != ns.get_xe() || y_end != ns.get_ye();
    }

    const int& get_xs() const { return x_start; }
    const int& get_ys() const { return y_start; }
    const int& get_xe() const { return   x_end; }
    const int& get_ye() const { return   y_end; }
    const int& get_layer() const { return layer; }
    CoordPair get_start() const { return CoordPair(x_start, y_start); }
    CoordPair get_end()   const { return CoordPair(x_end,   y_end);   }
    // direction: true -> vertical, false -> horizontal
    bool get_direction() const { return (x_start < x_end) ? true : false; }
    unsigned get_length() const { return (x_end - x_start) + (y_end - y_start); }

    bool check_overlap(const NetSegment& ns) const {
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
    CoordPair check_shared_point(const NetSegment& ns) const {
        if(x_start == ns.get_xs() && y_start == ns.get_ys()) return CoordPair(x_start, y_start);
        if(x_start == ns.get_xe() && y_start == ns.get_ye()) return CoordPair(x_start, y_start);
        if(x_end   == ns.get_xs() && y_end   == ns.get_ys()) return CoordPair(  x_end,   y_end);
        if(x_end   == ns.get_xe() && y_end   == ns.get_ye()) return CoordPair(  x_end,   y_end);
        return CoordPair(-1, -1);
    }
    CoordPair get_instersect(const NetSegment& ns) const {
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
    bool check_instersect(const NetSegment& ns) const { return get_instersect(ns) != CoordPair(-1, -1); }

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
    NetSegment split_segment(CoordPair& coord) { // split segment
        if(get_direction() && x_start < coord.first && x_end > coord.first){ // vertical (difference: x)
            NetSegment splitted(coord.first, y_start, x_end, y_end, layer);
            x_end = coord.first;
            return splitted;
        } else if (!get_direction() && y_start < coord.second && y_end > coord.second) { // horizontal
            NetSegment splitted(x_start, coord.second, x_end, y_end, layer);
            y_end = coord.second;
            return splitted;
        } else {
            return NetSegment();
        }
    }

   private:
    int x_start, y_start, x_end, y_end, layer;
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
int clamp(int _x, int _min, int _max){
    assert(_min <= _max);
    if(_x < _min) return _min;
    if(_x > _max) return _max;
    return _x;
}
int ABS(int _x){ return _x > 0 ? _x : -_x; }
int MIN(int _x, int _y){ return _x < _y ? _x : _y; }
int MAX(int _x, int _y){ return _x > _y ? _x : _y; }
unsigned dir2Num(const std::string s){
    if(s.compare("up") == 0)    return 1;
    if(s.compare("down") == 0)  return 2;
    if(s.compare("left") == 0)  return 3;
    if(s.compare("right") == 0) return 4;
    assert(false);
    return 0;
}
std::string num2Dir(const unsigned dir){
    if(dir == 1) return "up";
    if(dir == 2) return "down";
    if(dir == 3) return "left";
    if(dir == 4) return "right";
    assert(false);
    return 0;
}
unsigned opposite_dir(unsigned dir){
    if(dir == 1) return 2;
    if(dir == 2) return 1;
    if(dir == 3) return 4;
    if(dir == 4) return 3;
    assert(false);
    return 0;
}
unsigned opposite_dir(const std::string s){
    return opposite_dir(dir2Num(s));
}
