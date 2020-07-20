// * Copyright (C) Ren-Chu Wang, Yang Chien-Yi - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "ConjugateGradient.h"

#include <assert.h>
#include <math.h>

#include <algorithm>
#include <memory>

Scheduler::Scheduler(double i) noexcept : init(i), times(0) {}

Scheduler::Scheduler(Scheduler&& sch) noexcept
    : init(sch.init), times(sch.times) {}

double Scheduler::next(void) {
    ++times;
    return init / (double)times;
}

// ! reference:
// ! https://en.wikipedia.org/wiki/Nonlinear_conjugate_gradient_method

ConjGrad::ConjGrad(Chip& chip,
                   QuadForest& qf,
                   GradType gt,
                   unsigned times,
                   Scheduler&& sch) noexcept
    : chip(chip),
      qf(qf),
      times(times),
      coeff(true),
      current_best(-1.),
      gt(gt),
      best_step(0.),
      sch(std::move(sch)), 
      cst(chip){
    size_t size = dim();
    grads = safe::vector<double>(size, 0.);
    prev_grads = safe::vector<double>(size, 0.);

    pos.reserve(2 * chip.getNumCells());
    for (unsigned i = 0; i < chip.getNumCells(); ++i) {
        const Cell& cell = chip.getCell(i);
        pos.push_back((double)cell.getColumn());
        pos.push_back((double)cell.getRow());
    }
}

ConjGrad::ConjGrad(Chip& chip,
                   QuadForest& qf,
                   GradType gt,
                   unsigned times,
                   double init) noexcept
    : chip(chip),
      qf(qf),
      times(times),
      coeff(true),
      current_best(-1.),
      gt(gt),
      best_step(0.),
      sch(Scheduler(init)), 
      cst(chip) {
    size_t size = dim();
    grads = safe::vector<double>(size, 0.);
    prev_grads = safe::vector<double>(size, 0.);

    pos.reserve(2 * chip.getNumCells());
    for (unsigned i = 0; i < chip.getNumCells(); ++i) {
        const Cell& cell = chip.getCell(i);
        pos.push_back((double)cell.getColumn());
        pos.push_back((double)cell.getRow());
    }
}

size_t ConjGrad::dim(void) const {
    return 2 * chip.getNumCells();
}

safe::vector<double>& ConjGrad::positions(void) {
    return pos;
}

const safe::vector<double>& ConjGrad::positions(void) const {
    return pos;
}

void ConjGrad::apply(void) {
    auto iter = pos.begin();
    for (size_t i = 0; i < chip.getNumCells(); ++i) {
        Cell& cell = chip.getCell(i);
        cell.setColumn((unsigned)round(*(iter++)));
        cell.setRow((unsigned)round(*(iter++)));
    }

    assert(pos.size() == dim());
    assert(iter == pos.end());
}

unsigned ConjGrad::HPWL() {
    return cst.HPWL(pos);
}

void ConjGrad::mv(void) {
    prev_grads = std::move(grads);
    grads = safe::vector<double>(dim(), 0.);
}

void ConjGrad::update_directions(void) {
    assert(grads.size() == dim());
    assert(prev_grads.size() == dim());

    auto grad_iter = grads.begin();
    auto prev_iter = prev_grads.begin();

    double b = beta();

    for (; grad_iter != grads.end(); ++grad_iter, ++prev_iter) {
        *grad_iter = (*grad_iter) + b * (*prev_iter);
    }

    assert(grad_iter == grads.end());
    assert(prev_iter == prev_grads.end());
}

void ConjGrad::update_positions(void) {
    assert(grads.size() == dim());
    assert(pos.size() == dim());

    double step_size = sch.next();

    auto giter = grads.begin();
    auto piter = pos.begin();

    if (coeff) {
        for (; giter != grads.end(); ++giter, ++piter) {
            *piter -= step_size * (*giter);
        }
    } else {
        for (; giter != grads.end(); ++giter, ++piter) {
            *piter += step_size * (*giter);
        }
    }

    assert(giter == grads.end());
    assert(piter == pos.end());
}

void ConjGrad::all(void) {
    current_best = value_and_grad();

    for (unsigned i = 0; i < times; ++i) {
        if (!all_once()) {
            coeff = !coeff;
        }
    }
}

bool ConjGrad::all_once(void) {
    // calculate value and gradients
    // values are returned
    // gradients are stored in `grads`
    double val = value();

    update_positions();

    assert(current_best >= 0.);
    if (val < current_best) {
        current_best = val;
        return true;
    } else {
        return false;
    }
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

double ConjGrad::beta(void) const {
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

double ConjGrad::value(void) {
    double ret;
    switch (gt) {
        case GradType::Plain:
            // TODO calculate final value
            cst.evaluateF(pos, ret);
            return ret;
    }
    return ILLEGAL;
}

double ConjGrad::value_and_grad(void) {
    double ret;
    switch (gt) {
        case GradType::Plain:
            // TODO calculate gradient
            cst.evaluateFG(pos, ret, grads);
            return ret;
    }
    return ILLEGAL;
}
