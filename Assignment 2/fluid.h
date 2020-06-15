#ifndef FLUID_H
#define FLUID_H

#include<iostream>
#include "optimal.h"
#include "Polygon.h"
#include "lloyd.h"

using namespace std;

#define g Point(0.0,-9.8)
//number of particles
#define N 100
#define iters 100

void Move(vector<Point> &Ps, vector<Point> &v, vector<double> mass, vector<double> W, double eps=0.004, double dt=0.002){
    vector<Polygon> Poly=SHDiagram(Ps,W);
    for (int i=0;i<Ps.size();i++){
        if (mass[i]>0){//Assum only water moves but air not.
            //cout<<"here"<<endl;
            Point F=(centroid(Poly[i])-Ps[i])/(eps*eps)+g;
            Ps[i]=Ps[i]+v[i]*dt;
            v[i]=v[i]+F*dt/mass[i];
            // if the particle goes out of the box
            if (Ps[i].x < 0) {
                Ps[i].x = - Ps[i].x;
                v[i].x = - v[i].x;
            }
            if (Ps[i].x > 1) {
                Ps[i].x = 2 - Ps[i].x;
                v[i].x = - v[i].x;
            }
            if (Ps[i].y < 0) {
                Ps[i].y = - Ps[i].y;
                v[i].y = - v[i].y;
            }
            if (Ps[i].y > 1) {
                Ps[i].y = 2 - Ps[i].y;
                v[i].y = - v[i].y;
            }
        }
    }
}

void Fluid(){
    vector<Point> Ps=lloyd(N);
    vector<Point> v(N,Point(0,0));
    vector<double> mass(N,-1);//if it is water it has mass bigger than 0 air->-1;
    // we assum that it is a very thin space with 1 L volume, 
    // so every point of water we asume its volume is 1/N L 
    // so mass is 1/N kg;
    Point circle(1.0/2,2.0/5);
    //cout<<circle<<endl;
    double rsq=1.0/16;//initial water circle
    vector<double> ws(N, 1/N);
    for (int i=0;i<N;i++){
        //cout<<Ps[i];
        if (dist(Ps[i],circle)<=rsq) mass[i]=200.0;
    }
    cout<<mass;
    return;
    vector<Polygon> Polys=SHDiagram(Ps,ws);
    save_svg_p(Polys,Ps,"init.svg");
    vector<double> W;
    for (int i=0;i<N;i++){
        W.push_back(0.01);
    }
    for (int i=0;i<iters;i++){
        OTweight(Ps,W,"uniform");
        Move(Ps,v,W,mass);
        //cout<<Ps;
        vector<Polygon> Polys=SHDiagram(Ps,ws);
        save_svg_animated(Polys,"fluid.svg", i, iters);
    }
}


#endif