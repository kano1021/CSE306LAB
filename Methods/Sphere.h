#ifndef Sphere_H
#define Sphere_H

#include<cmath>
#include<string>
#include "Vector.h"
#include "Frame.h"
#include<iostream>

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
        Vector C = this->C;
        double R = this->R;
        Intersection its;
        double b=dot(ray.u, ray.O - C);
        double c=dot(ray.O - C, ray.O - C) - pow(R,2);
        double delta = pow(b, 2) - c;
        if (delta<0) its.occured=false;
        else{
            its.occured=true;
            //cout<<"Here 1"<<endl;
            double t1 = -b - sqrt(delta);
            double t2 = -b + sqrt(delta);
        
            if (t2 < 0) {
                its.occured = false;
            }else {
                //cout<<"Here 1"<<endl;
                if (t1 >= 0) {
                    its.t = t1;
                }else{
                    its.t = t2;
                }
            }
        }
        its.P = ray.O + ray.u*its.t;
        //cout<<"[" << its.P[0] << " "<< its.P[1] << " "<<its.P[2] <<"]"<<endl;
        its.N = normalize(its.P - C);

        return its;
    }
};

#endif