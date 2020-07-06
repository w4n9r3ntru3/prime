// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "ConjugateGradient.h"

#include <assert.h>

#include <algorithm>
#include <memory>

ConjugateGradient::ConjugateGradient(void) noexcept
    : grads(safe::vector<Gradient>()), prev_grads(safe::vector<Gradient>()) {}

ConjugateGradient::ConjugateGradient(size_t size) noexcept
    : grads(safe::vector<Gradient>(size)),
      prev_grads(safe::vector<Gradient>(size)) {}

ConjugateGradient::ConjugateGradient(safe::vector<Gradient>&& g) noexcept
    : grads(std::move(g)), prev_grads(safe::vector<Gradient>()) {}

ConjugateGradient::ConjugateGradient(safe::vector<Gradient>&& g,
                                     safe::vector<Gradient>&& pg) noexcept
    : grads(std::move(g)), prev_grads(std::move(pg)) {}

ConjugateGradient::ConjugateGradient(const QuadForest& qf) noexcept
    : grads(safe::vector<Gradient>(qf.size())),
      prev_grads(safe::vector<Gradient>(qf.size())) {
    assert(grads.size() == qf.size());
    assert(prev_grads.size() == qf.size());
}

size_t ConjugateGradient::size(void) const {
    assert(prev_grads.size() == grads.size());
    return grads.size();
}

void ConjugateGradient::clear(void) {
    prev_grads.clear();
    grads.clear();
}

void ConjugateGradient::zero_grad_(void) {
    std::fill(grads.begin(), grads.end(), Gradient(0., 0.));
    std::fill(prev_grads.begin(), prev_grads.end(), Gradient(0., 0.));
}
