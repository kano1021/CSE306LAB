#ifndef Sphere_H
#define Sphere_H

#include<cmath>
#include<string>
#include "Vector.h"
#include "Frame.h"
using namespace std;

class Sphere{
public: 
    Vector C;
    double R;
    Vector albedo;
    bool mirror;
    Sphere(Vector C, double R, Vector albedo, bool mirror=false){
        this->C=C;
        this->R=R;
        this->albedo=albedo;
        this->mirror=mirror;
    }
    Intersection intersect(const Ray& ray){
        Intersection its;
        Vector O_C = ray.O - C; 
        double b=dot(ray.u, O_C);
        double c=dot(O_C, O_C) - R*R;
        double delta = pow(b, 2) - c;
        if (delta<0) its.occured=false;
        else its.occured=true;

        if (its.occured) {
            double t1 = -b - sqrt(delta);
            double t2 = -b + sqrt(delta);

            if (t2 < 0) {
                its.occured = false;
            }else {
                if (t1 >= 0) {
                    its.t = t1;
                }else{
                    its.t = t2;
                }
            }
        }
        its.P = ray.O + ray.u*its.t;
        its.N = normalize(its.P - C);

        return its;
    }
};

#endif