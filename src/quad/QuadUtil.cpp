// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "QuadUtil.h"

// NetSegment
NetSegment::NetSegment() noexcept
    : x_start(-1),
      y_start(-1),
      x_end(-1),
      y_end(-1),
      layer_start(-1),
      layer_end(-1) {}

NetSegment::NetSegment(int xs, int ys, int xe, int ye, int l) noexcept
    : x_start(xs),
      y_start(ys),
      x_end(xe),
      y_end(ye),
      layer_start(l),
      layer_end(l) {
    if (x_start > x_end || y_start > y_end) {
        std::swap(x_start, x_end);
        std::swap(y_start, y_end);
    }
    assert(x_start <= x_end);
    assert(y_start <= y_end);
}
NetSegment::NetSegment(int xs, int ys, int xe, int ye, int l, int le) noexcept
    : x_start(xs),
      y_start(ys),
      x_end(xe),
      y_end(ye),
      layer_start(l),
      layer_end(le) {
    if (x_start > x_end || y_start > y_end) {
        std::swap(x_start, x_end);
        std::swap(y_start, y_end);
    }
    if (layer_start > layer_end) {
        std::swap(layer_start, layer_end);
    }
    assert(x_start <= x_end);
    assert(y_start <= y_end);
    assert(layer_start <= layer_end);
}

bool NetSegment::operator<(const NetSegment& ns) const {
    if (x_start < ns.get_xs())
        return true;
    else if (x_start > ns.get_xs())
        return false;
    else if (x_end < ns.get_xe())
        return true;
    else if (x_end > ns.get_xe())
        return false;
    else if (y_start < ns.get_ys())
        return true;
    else if (y_start > ns.get_ys())
        return false;
    else if (y_end < ns.get_ye())
        return true;
    else if (y_end > ns.get_ye())
        return false;
    else if (layer_start < ns.get_layer())
        return true;
    else
        return true;
}
bool NetSegment::operator!=(const NetSegment& ns) const {
    return x_start != ns.get_xs() || y_start != ns.get_ys() ||
           x_end != ns.get_xe() || y_end != ns.get_ye();
}

int NetSegment::get_xs() const {
    return x_start;
}
int NetSegment::get_ys() const {
    return y_start;
}
int NetSegment::get_xe() const {
    return x_end;
}
int NetSegment::get_ye() const {
    return y_end;
}
int NetSegment::get_layer() const {
    return layer_start;
}
int NetSegment::get_layer_end() const {
    return (layer_end < 0) ? layer_start : layer_end;
}
CoordPair NetSegment::get_start() const {
    return CoordPair(x_start, y_start);
}
CoordPair NetSegment::get_end() const {
    return CoordPair(x_end, y_end);
}
bool NetSegment::get_direction() const {
    return (x_start < x_end) ? true : false;
}
unsigned NetSegment::get_length() const {
    return ABS(x_end - x_start) + ABS(y_end - y_start);
}

bool NetSegment::check_overlap(const NetSegment& ns) const {
    // Check whether two parallel segments overlap
    if (get_direction() != ns.get_direction())
        return false;
    if (get_direction() && y_start == ns.get_ys()) {  // vertical
        if (x_start <= ns.get_xs() && x_end >= ns.get_xs())
            return true;
        if (x_start >= ns.get_xs() && x_start <= ns.get_xe())
            return true;
    } else if (!get_direction() && x_start == ns.get_xs()) {  // horizontal
        if (y_start <= ns.get_ys() && y_end >= ns.get_ys())
            return true;
        if (y_start >= ns.get_ys() && y_start <= ns.get_ye())
            return true;
    }
    return false;
}
CoordPair NetSegment::check_shared_point(const NetSegment& ns) const {
    if (x_start == ns.get_xs() && y_start == ns.get_ys())
        return CoordPair(x_start, y_start);
    if (x_start == ns.get_xe() && y_start == ns.get_ye())
        return CoordPair(x_start, y_start);
    if (x_end == ns.get_xs() && y_end == ns.get_ys())
        return CoordPair(x_end, y_end);
    if (x_end == ns.get_xe() && y_end == ns.get_ye())
        return CoordPair(x_end, y_end);
    return CoordPair(-1, -1);
}
CoordPair NetSegment::get_instersect(const NetSegment& ns) const {
    // Find the intersection of the two orthogonal segments
    if (get_direction() == ns.get_direction())
        return CoordPair(-1, -1);
    if (check_shared_point(ns) != CoordPair(-1, -1))
        return CoordPair(-1, -1);
    if (get_direction()) {  // vertical
        if (x_start <= ns.get_xs() && ns.get_xs() <= x_end &&
            ns.get_ys() <= y_start && y_start <= ns.get_ye()) {
            return CoordPair(ns.get_xs(), y_start);
        }
    } else {  // horizontal
        if (y_start <= ns.get_ys() && ns.get_ys() <= y_end &&
            ns.get_xs() <= x_start && x_start <= ns.get_xe()) {
            return CoordPair(x_start, ns.get_ys());
        }
    }
    return CoordPair(-1, -1);
}
bool NetSegment::check_instersect(const NetSegment& ns) const {
    return get_instersect(ns) != CoordPair(-1, -1);
}
bool NetSegment::contains(const CoordPair& cp) const {
    return x_start <= cp.first && x_end >= cp.first && y_start <= cp.second &&
           y_end >= cp.second;
}

void NetSegment::merge_segment(NetSegment& ns) {  // merge two parallel segments
    if (!check_overlap(ns))
        return;
    if (get_direction()) {  // vertical
        if (x_start > ns.get_xs())
            x_start = ns.get_xs();
        if (x_end < ns.get_xe())
            x_end = ns.get_xe();
    } else {  // horizontal
        if (y_start > ns.get_ys())
            y_start = ns.get_ys();
        if (y_end < ns.get_ye())
            y_end = ns.get_ye();
    }
}
NetSegment NetSegment::split_segment(CoordPair& coord) {  // split segment
    if (get_direction() && x_start < coord.first &&
        x_end > coord.first) {  // vertical (difference: x)
        NetSegment splitted(coord.first, y_start, x_end, y_end, layer_start);
        x_end = coord.first;
        return splitted;
    } else if (!get_direction() && y_start < coord.second &&
               y_end > coord.second) {  // horizontal
        NetSegment splitted(x_start, coord.second, x_end, y_end, layer_start);
        y_end = coord.second;
        return splitted;
    } else {
        return NetSegment();
    }
}

std::ostream& operator<<(std::ostream& out, const NetSegment& ns) {
    out << "(" << ns.get_xs() << ", " << ns.get_ys() << ") "
        << "(" << ns.get_xe() << ", " << ns.get_ye() << ") "
        << "layer = " << ns.get_layer();
    return out;
}

std::tuple<NetSegment, NetSegment, NetSegment> merge_segments(
    const NetSegment& ns1,
    const NetSegment& ns2,
    const unsigned importance) {
    // Merge segments ns1 and ns2
    // After merging, there might be three segments
    int priority_layer = (importance) ? ns1.layer_start : ns2.layer_start;
    if (ns1.get_direction() && ns2.get_direction() &&
        ns1.y_start == ns2.y_start) {  // Vertical
        if (ns1.x_start <= ns2.x_start && ns2.x_end <= ns1.x_end) {
            // Case 1 : ns1 contains the whole segment ns2
            // 1: *---------*
            // 2:    *----*
            return std::make_tuple(
                NetSegment(ns1.x_start, ns1.y_start, ns2.x_start, ns2.y_start,
                           ns1.layer_start),
                NetSegment(ns2.x_start, ns2.y_start, ns2.x_end, ns2.y_end,
                           priority_layer),
                NetSegment(ns2.x_end, ns2.y_end, ns1.x_end, ns1.y_end,
                           ns1.layer_start));
        } else if (ns2.x_start <= ns1.x_start && ns1.x_end <= ns2.x_end) {
            // Case 2 : ns2 contains the whole segment ns1
            // 1:    *----*
            // 2: *---------*
            return merge_segments(ns2, ns1, importance ^ 1);
        } else if (ns1.x_start < ns2.x_start && ns2.x_start <= ns1.x_end &&
                   ns1.x_end < ns2.x_end) {
            // Case 3 : ns1 has overlap with ns2
            // 1: *---------*
            // 2:    *-----------*
            return std::make_tuple(
                NetSegment(ns1.x_start, ns1.y_start, ns2.x_start, ns2.y_start,
                           ns1.layer_start),
                NetSegment(ns2.x_start, ns2.y_start, ns1.x_end, ns1.y_end,
                           priority_layer),
                NetSegment(ns1.x_end, ns1.y_end, ns2.x_end, ns2.y_end,
                           ns2.layer_start));
        } else if (ns2.x_start < ns1.x_start && ns1.x_start <= ns2.x_end &&
                   ns2.x_end < ns1.x_end) {
            // Case 4 : ns1 has overlap with ns2
            // 1:      *---------*
            // 2: *-----------*
            return merge_segments(ns2, ns1, importance ^ 1);
        }
    } else if (!ns1.get_direction() && !ns2.get_direction() &&
               ns1.x_start == ns2.x_start) {  // Horizontal
        if (ns1.y_start <= ns2.y_start && ns2.y_end <= ns1.y_end) {
            // Case 1 : ns1 contains the whole segment ns2
            return std::make_tuple(
                NetSegment(ns1.x_start, ns1.y_start, ns2.x_start, ns2.y_start,
                           ns1.layer_start),
                NetSegment(ns2.x_start, ns2.y_start, ns2.x_end, ns2.y_end,
                           priority_layer),
                NetSegment(ns2.x_end, ns2.y_end, ns1.x_end, ns1.y_end,
                           ns1.layer_start));
        } else if (ns2.y_start <= ns1.y_start && ns1.y_end <= ns2.y_end) {
            // Case 2 : ns2 contains the whole segment ns1
            return merge_segments(ns2, ns1, importance ^ 1);
        } else if (ns1.y_start < ns2.y_start && ns2.y_start <= ns1.y_end &&
                   ns1.y_end < ns2.y_end) {
            // Case 3 : ns1 has overlap with ns2
            return std::make_tuple(
                NetSegment(ns1.x_start, ns1.y_start, ns2.x_start, ns2.y_start,
                           ns1.layer_start),
                NetSegment(ns2.x_start, ns2.y_start, ns1.x_end, ns1.y_end,
                           priority_layer),
                NetSegment(ns1.x_end, ns1.y_end, ns2.x_end, ns2.y_end,
                           ns2.layer_start));
        } else if (ns2.y_start < ns1.y_start && ns1.y_start <= ns2.y_end &&
                   ns2.y_end < ns1.y_end) {
            // Case 4 : ns1 has overlap with ns2
            return merge_segments(ns2, ns1, importance ^ 1);
        }
    }
    return std::make_tuple(ns1, ns2, NetSegment());
}

// SimpleUnionFind
SimpleUnionFind::SimpleUnionFind() noexcept {}
SimpleUnionFind::SimpleUnionFind(size_t N) noexcept {
    reset(N);
}
SimpleUnionFind::~SimpleUnionFind() noexcept {
    clear();
}

inline unsigned SimpleUnionFind::find(unsigned x) {
    return (x == parent[x]) ? x : parent[x] = find(parent[x]);
}
bool SimpleUnionFind::same(unsigned x, unsigned y) {
    return find(x) == find(y);
}
void SimpleUnionFind::merge(unsigned x, unsigned y) {
    x = find(x);
    y = find(y);
    if (x == y)
        return;
    if (rank[x] < rank[y]) {
        parent[x] = y;
    } else {
        if (rank[x] == rank[y])
            ++rank[x];
        parent[y] = x;
    }
}
bool SimpleUnionFind::check_all_merged() {
    for (size_t i = 1; i < parent.size(); ++i) {
        if (!same(0, i))
            return false;
    }
    return true;
}
unsigned SimpleUnionFind::first_not_merged() {
    assert(!check_all_merged());
    for (size_t i = 1; i < parent.size(); ++i) {
        if (!same(0, i))
            return i;
    }
    return 0;
}
void SimpleUnionFind::clear() {
    parent.clear();
    rank.clear();
}
void SimpleUnionFind::reset(size_t N) {
    clear();
    parent.resize(N);
    rank.resize(N);
    for (size_t i = 0; i < N; ++i)
        parent[i] = i;
}

void SimpleUnionFind::reserve(size_t N) {
    parent.reserve(N);
    rank.reserve(N);
}

// SimplePin

SimplePin::SimplePin() noexcept : idx(0), row(0), column(0), layer(0) {}
SimplePin::SimplePin(unsigned _idx,
                     unsigned _row,
                     unsigned _col,
                     unsigned _lay) noexcept
    : idx(_idx), row(_row), column(_col), layer(_lay) {}

unsigned SimplePin::get_idx() const {
    return idx;
}
unsigned SimplePin::get_row() const {
    return row;
}
unsigned SimplePin::get_col() const {
    return column;
}
unsigned SimplePin::get_layer() const {
    return layer;
}

// Utility functions
int clamp(int _x, int _min, int _max) {
    assert(_min <= _max);
    if (_x < _min)
        return _min;
    if (_x > _max)
        return _max;
    return _x;
}
int ABS(int _x) {
    return _x > 0 ? _x : -_x;
}
int MIN(int _x, int _y) {
    return _x < _y ? _x : _y;
}
int MAX(int _x, int _y) {
    return _x > _y ? _x : _y;
}
unsigned dir2Num(const std::string s) {
    if (s.compare("up") == 0)
        return 1;
    if (s.compare("down") == 0)
        return 2;
    if (s.compare("left") == 0)
        return 3;
    if (s.compare("right") == 0)
        return 4;
    assert(false);
    return 0;
}
std::string num2Dir(const unsigned dir) {
    if (dir == 1)
        return "up";
    if (dir == 2)
        return "down";
    if (dir == 3)
        return "left";
    if (dir == 4)
        return "right";
    assert(false);
    return 0;
}
unsigned opposite_dir(unsigned dir) {
    if (dir == 1)
        return 2;
    if (dir == 2)
        return 1;
    if (dir == 3)
        return 4;
    if (dir == 4)
        return 3;
    assert(false);
    return 0;
}
unsigned opposite_dir(const std::string s) {
    return opposite_dir(dir2Num(s));
}
