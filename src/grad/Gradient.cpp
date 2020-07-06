// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "Gradient.h"

#include <iostream>

Gradient::Gradient(void) noexcept : horiz(0.), verti(0.) {}

Gradient::Gradient(double a, double b) noexcept : horiz(a), verti(b) {}

double Gradient::x(void) const {
    return horiz;
}

double Gradient::y(void) const {
    return verti;
}

void Gradient::x(double v) {
    horiz = v;
}

void Gradient::y(double v) {
    verti = v;
}
