// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "ConjugateGradient.h"

#include <assert.h>

#include <algorithm>
#include <memory>

ConjugateGradient::ConjugateGradient(Chip& chip, QuadForest& qf) noexcept
    : chip(chip), qf(qf) {
    size_t size = dim();
    grads = safe::vector<double>(size, 0.);
    prev_grads = safe::vector<double>(size, 0.);
}

size_t ConjugateGradient::dim(void) const {
    size_t size = 2 * chip.getNumCells();
    return size;
}

void ConjugateGradient::zero_grad_(void) {
    std::fill(grads.begin(), grads.end(), 0.);
}

double ConjugateGradient::eval(GradientType gt) {
    switch (gt) {
        case GradientType::Plain:
            // todo: collect values
            break;
    }
    return 0.;
}
void ConjugateGradient::backward(GradientType gt) {
    switch (gt) {
        case GradientType::Plain:
            // todo: collect gradients
            break;
    }
}

void ConjugateGradient::move(void) {
    prev_grads = std::move(grads);
    grads = safe::vector<double>(dim(), 0.);
}

static double dot(const safe::vector<double>& left,
                  const safe::vector<double>& right) {
    assert(left.size() == right.size());

    double sum = 0;
    auto liter = left.begin(), riter = right.begin();
    for (; liter != left.end() && riter != right.end(); ++liter, ++riter) {
        sum += (*liter) * (*riter);
    }
    assert(liter == left.end() && riter == right.end());
    return sum;
}

double ConjugateGradient::beta(void) const {
    safe::vector<double> buffer(grads);
    auto niter = buffer.begin();
    auto piter = prev_grads.begin();
    for (; niter != buffer.end(); ++niter, ++piter) {
        (*niter) -= (*piter);
    }
    assert(niter == buffer.end() && piter == prev_grads.end());

    double numerator = dot(grads, buffer);

    if (numerator <= 0) {
        return 0.;
    } else {
        return numerator / dot(prev_grads, prev_grads);
    }
}
