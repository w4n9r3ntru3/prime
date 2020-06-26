/*
This file is to define the basic grids in a die for routing.
*/

#ifndef GRID
#define GRID

enum GridType { Normal_Grid, Start_Grid, End_Grid, Block_Grid, Reserve_Grid };

class Grid {
   public:
    Grid(unsigned i) : _idx(i) {}
    ~Grid() {}
    // pure virtual functions
    virtual GridType type() const = 0;
    virtual bool routable(const unsigned) const = 0;
    virtual int get_cost() const = 0;
    virtual int get_estimated() const = 0;
    virtual unsigned get_target() const = 0;
    virtual void assign_estimated(unsigned, int) = 0;
    virtual void assign_cost(int) = 0;
    virtual void reset() = 0;

    // functions
    unsigned get_idx() const { return _idx; }

   private:
    unsigned _idx;
};

#endif

#ifndef NORMALGRID
#define NORMALGRID

class NormalGrid : public Grid {
   public:
    NormalGrid(unsigned i) : Grid(i), _cost(0), _search(0), _nearPin(0) {}
    NormalGrid(unsigned i, unsigned pi)
        : Grid(i), _cost(0), _search(0), _nearPin(1), _pin(pi) {}
    ~NormalGrid() {}
    GridType type() const override { return Normal_Grid; }
    bool routable(const unsigned target) const override {
        if (_nearPin && target != _pin)
            return false;
        return !_search;
    }
    int get_cost() const override { return _cost; }
    int get_estimated() const override { return _estimated_cost; }
    unsigned get_target() const override {
        return _pi;
    }  // should be get_pi, only for virtual convenience
    void assign_cost(int cost) override { _cost = cost; }
    void assign_estimated(unsigned pi, int cost) override {
        _pi = pi;
        _estimated_cost = cost;
        _search = true;
    }
    void reset() override { _search = false; }

   private:
    int _cost;
    unsigned _pi;
    bool _nearPin;
    unsigned _pin;
    int _estimated_cost;
    bool _search;
    // bool                _nearby;
    // bool                _init;
};

#endif

#ifndef STARTGRID
#define STARTGRID

class StartGrid : public Grid {
   public:
    StartGrid(unsigned i, unsigned e) : Grid(i), _target(e) {}
    ~StartGrid() {}
    // void add_end(unsigned i) { _EndList.push_back(i); }
    GridType type() const override { return Start_Grid; }
    bool routable(const unsigned a) const override { return false; }
    int get_cost() const override { return 0; }
    unsigned get_target() const override { return _target; }
    void reset() override { return; }
    virtual int get_estimated() const { return 0; }
    virtual void assign_estimated(unsigned a, int b) {}
    virtual void assign_cost(int a) {}

   private:
    unsigned _target;
};

#endif

#ifndef ENDGRID
#define ENDGRID

class EndGrid : public Grid {
   public:
    EndGrid(unsigned i) : Grid(i) {}
    ~EndGrid() {}
    GridType type() const override { return End_Grid; }
    bool routable(const unsigned a) const override { return false; }
    int get_cost() const override { return _cost; }
    void assign_estimated(unsigned pi, int c) override {
        _pi = pi;
        _cost = c;
    }  // this is for virtual convenience
    unsigned get_target() const override {
        return _pi;
    }  // should be get_pi, but for virtual convenience
    void reset() override { return; }
    int get_estimated() const { return _cost; }
    void assign_cost(int a) {}

   private:
    unsigned _pi;
    int _cost;
};

#endif

#ifndef BLOCKGRID
#define BLOCKGRID

class BlockGrid : public Grid {
   public:
    BlockGrid(unsigned i) : Grid(i) {}
    ~BlockGrid() {}
    GridType type() const override { return Block_Grid; }
    bool routable(const unsigned a) const override { return false; }
    void reset() override { return; }
    int get_cost() const { return 0; }
    int get_estimated() const { return 0; }
    unsigned get_target() const { return 0; }
    void assign_estimated(unsigned a, int b) {}
    void assign_cost(int a) {}

   private:
};

#endif

#ifndef RESERVEGRID
#define RESERVEGRID

class ReserveGrid : public Grid {
   public:
    ReserveGrid(unsigned i) : Grid(i) {}
    ~ReserveGrid() {}
    GridType type() const override { return Reserve_Grid; }
    bool routable(const unsigned a) const override { return false; }
    void reset() override { return; }
    int get_cost() const { return 0; }
    int get_estimated() const { return 0; }
    unsigned get_target() const { return 0; }
    void assign_estimated(unsigned a, int b) {}
    void assign_cost(int a) {}

   private:
};

#endif