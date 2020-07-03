/*
The file is to define the cost functions
*/
#include <iostream>
#include "../include/router3d.h"

#define max_diff(a, b) ((a) > (b) ? (a - b) : (b - a))

int Router3D::get_cost(unsigned a, unsigned b) {
    switch (_CostType) {
        case Manhattan:
            return ManDist(a, b);
            break;
        default:
            return 0;
            break;
    }
}

int Router3D::ManDist(unsigned a, unsigned b) const {
    unsigned x_a = get_row(a);
    unsigned x_b = get_row(b);
    unsigned y_a = get_column(a);
    unsigned y_b = get_column(b);
    unsigned z_a = get_layer(a);
    unsigned z_b = get_layer(b);
    unsigned x = max_diff(x_a, x_b);
    unsigned y = max_diff(y_a, y_b);
    unsigned z = max_diff(z_a, z_b);
    return x + y + z;
}