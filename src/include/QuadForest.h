// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>
#include <assert.h>

#include "QuadTree.h"

// Manage all nets
class QuadForest{
   public:
    QuadForest() noexcept;
    QuadForest(Chip& chip) noexcept;
    // ~QuadForest() noexcept;

    void construct_forest(Chip& chip);

    size_t size() const;
    QuadTree& get_tree(size_t idx);
    // QuadTree& get_tree(std::string s);
    QuadTree& operator[](size_t idx);
    const QuadTree& operator[](size_t idx) const;
    QuadTree& get_tree_by_pin(size_t idx);

    void push_back(const QuadTree& qt);
    void push_back(QuadTree&& qt);

    // void clear();
    void return_segments(Chip& chip);

   private:
    int                     baseRowIdx;
    int                     baseColIdx;
    int                        maxRows;
    int                        maxCols;
    int                      maxLayers;
    safe::vector<QuadTree>      qtrees;
    // safe::unordered_map<std::string, unsigned> name2NetIdx;

    // safe::vector<CellPinPair>     pins;
    safe::vector<unsigned> pinIdx2Tree;

    // friend
    friend std::ostream& operator<<(std::ostream& out, QuadForest& qf);
};