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
    QuadForest(const Chip& chip) noexcept;
    ~QuadForest() noexcept;

    void construct_forest(const Chip& chip);

    unsigned size() const;
    QuadTree& get_tree(unsigned idx);

    void push_back(const QuadTree& qt);
    void push_back(QuadTree&& qt);

   private:
    
    safe::vector<QuadTree> qtrees;
};