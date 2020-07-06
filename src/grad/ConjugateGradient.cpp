// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "ConjugateGradient.h"

#include <assert.h>

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

size_t ConjugateGradient::size(void) const {
    assert(prev_grads.size() == grads.size());
    return grads.size();
}

void ConjugateGradient::clear(void) {
    prev_grads.clear();
    grads.clear();
}
