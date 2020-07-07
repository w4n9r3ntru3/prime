// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include "Chip.h"
#include "QuadForest.h"
#include "safe.h"

enum class GradientType { Plain };

class ConjugateGradient {
   public:
    explicit ConjugateGradient(void) = delete;
    explicit ConjugateGradient(Chip& chip, QuadForest& qf) noexcept;

    size_t dim(void) const;
    void zero_grad_(void);

    double eval(GradientType gt);
    void backward(GradientType gt);

    void move(void);

    void filter_grad(safe::unordered_set<unsigned>&& remaining);

   private:
    double beta(void) const;

    Chip& chip;
    QuadForest& qf;
    safe::vector<double> grads;
    safe::vector<double> prev_grads;
};
