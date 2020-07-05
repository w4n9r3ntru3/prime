// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

class Gradient {
   public:
    explicit Gradient(void);
    explicit Gradient(double x, double y);

    double x(void) const;
    double y(void) const;

    void x(double val);
    void y(double val);

   private:
    // gradient towards the top right
    double horiz, verti;
};
