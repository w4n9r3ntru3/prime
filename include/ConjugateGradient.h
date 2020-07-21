// * Copyright (C) Ren-Chu Wang, Yang Chien-Yi - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include "Chip.h"
#include "analytical.h"
#include "QuadForest.h"
#include "safe.h"

constexpr double ILLEGAL = -1.;

enum class GradType { Plain };

class Scheduler {
   public:
    // constructor
    Scheduler(void) noexcept = delete;
    Scheduler(double ini, unsigned t) noexcept;
    Scheduler(const Scheduler& sch) noexcept = delete;
    Scheduler(Scheduler&& sch) noexcept;

    // operator=
    Scheduler& operator=(const Scheduler& sch) noexcept = delete;
    Scheduler& operator=(Scheduler&& sch) noexcept;

    // other
    double next(void);

   private:
    double init;
    unsigned times;
};

class ConjGrad {
   public:
    // constructor
    ConjGrad(void) noexcept = delete;
    ConjGrad(Chip& chip,
             QuadForest& qf,
             GradType gt,
             unsigned times,
             Scheduler&& sch) noexcept;
    ConjGrad(Chip& chip,
             QuadForest& qf,
             GradType gt,
             unsigned times,
             double init) noexcept;

    // operator=
    ConjGrad& operator=(const ConjGrad& cg) noexcept;
    ConjGrad& operator=(ConjGrad&& cg) noexcept = delete;

    // do all
    void all(void);

    // other
    size_t dim(void) const;

    safe::vector<double>& positions(void);
    const safe::vector<double>& positions(void) const;

    void apply(void);

    unsigned HPWL();

    template <bool backward>
    double cg(void) {
        if (!backward) {
            return value();
        }

        mv();

        double v = value_and_grad();

        // assert(v == value());

        update_directions();

        update_positions();

        return v;
    }

   private:
    // do all once
    // if return true, do not revert the gradient
    // else revert gradient
    bool all_once(void);

    void mv(void);
    double beta(void) const;

    void update_directions(void);
    void update_positions(void);

    double value(void);
    double value_and_grad(void);

    void clip(void);

    Chip& chip;
    QuadForest& qf;
    safe::vector<double> grads;
    safe::vector<double> prev_grads;
    safe::vector<double> pos;
    unsigned times;
    bool coeff;
    double current_best;
    GradType gt;
    double best_step;
    Scheduler sch;
    Cost cst;
};
