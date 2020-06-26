/*
The file is to define the cost functions
*/
#include <iostream>
#include "die.h"

#define max_diff(a, b) ((a) > (b) ? (a - b) : (b - a))

int Die::get_cost(unsigned a, unsigned b) {
    switch (_CostType) {
        case Manhattan:
            return ManDist(a, b);
            break;
        default:
            return 0;
            break;
    }
}

int Die::ManDist(unsigned a, unsigned b) const {
    unsigned x_a = get_x(a);
    unsigned x_b = get_x(b);
    unsigned y_a = get_y(a);
    unsigned y_b = get_y(b);
    unsigned z_a = get_z(a);
    unsigned z_b = get_z(b);
    unsigned x = max_diff(x_a, x_b);
    unsigned y = max_diff(y_a, y_b);
    unsigned z = max_diff(z_a, z_b);
    return x + y + z;
}