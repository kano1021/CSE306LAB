#ifndef RAY_H
#define RAY_H

#include "Vector.h"

struct Ray {
    Vector O; 
    Vector u; 
    Ray(Vector O, Vector u) {
        this->O = O;
        this->u = u;
    }
};
#endif