#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Vector.h"

struct Intersection {
    bool occured;  
    double t;      
    Vector P;      
    Vector N;  
    Vector albedo; 
};
#endif