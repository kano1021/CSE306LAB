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
    double type;//-1 for difussion,0 for mirror, n>1 for glass
    int islight=0;
    Sphere(Vector C, double R, Vector albedo, double type=-1, int islight=0){
        this->C=C;
        this->R=R;
        this->albedo=albedo;
        this->type=type;
        this->islight=islight;
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