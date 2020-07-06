// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include "Gradient.h"
#include "safe.h"

class ConjugateGradient {
   public:
    explicit ConjugateGradient(void) noexcept;
    explicit ConjugateGradient(size_t size) noexcept;
    explicit ConjugateGradient(safe::vector<Gradient>&& grad) noexcept;
    explicit ConjugateGradient(safe::vector<Gradient>&& grad,
                               safe::vector<Gradient>&& prev_grads) noexcept;

    size_t size(void) const;
    void clear(void);

   private:
    safe::vector<Gradient> grads;
    safe::vector<Gradient> prev_grads;
};
