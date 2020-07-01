// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>
#include <assert.h>

#include "QuadTree.h"
#include "QuadNode.h"
#include "QuadUtil.h"

// Manage all nets
class QuadForest{
   public:
    QuadForest() noexcept;
    QuadForest(Chip& chip) noexcept;
    ~QuadForest() noexcept;

    void construct_forest(Chip& chip);

    unsigned size() const;
    QuadTree& get_tree(unsigned idx);
    QuadTree& get_tree(std::string s);

    void push_back(const QuadTree& qt);
    void push_back(QuadTree&& qt);

   private:
    int                        maxRows;
    int                        maxCols;
    int                      maxLayers;
    safe::vector<QuadTree>      qtrees;
    safe::unordered_map<std::string, unsigned> name2NetIdx;

    safe::vector<std::shared_ptr<Pin>> pins;
    safe::vector<unsigned>     pins_in_tree;
};