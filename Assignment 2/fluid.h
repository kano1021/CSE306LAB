#ifndef FLUID_H
#define FLUID_H

#include<iostream>
#include "optfluid.h"
#include "Polygon.h"
#include "lloyd.h"

using namespace std;

#define g Point(0.0,-9.8)
//number of particles
#define N 2000
#define iters 200

void Move(vector<Point> &Ps, vector<Point> &v, vector<double> mass, vector<double> W, double eps=0.004, double dt=0.002){
    vector<Polygon> Poly=SHDiagram(Ps,W);
    for (int i=0;i<Ps.size();i++){
        if (mass[i]>0){//Assum only water moves but air not.
            //cout<<"here"<<endl;
            Point F=(centroid(Poly[i])-Ps[i])/(eps*eps);
            Ps[i]=Ps[i]+v[i]*dt;
            v[i]=v[i]+F*dt/mass[i]+g*dt;   
        }else{
            Point F=(centroid(Poly[i])-Ps[i])/(eps*eps);
            Ps[i]=Ps[i]+v[i]*dt;
            v[i]=v[i]-F*dt/mass[i];  
        }
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

void Fluid(){
    vector<Point> Ps=lloyd(N);
    vector<Point> v(N,Point(0,0));
    vector<double> mass(N,-20);//if it is water it has mass=200 air->-20;
    //(volume air is much lighter than water)
    // we assum that it is a very thin space with 1 L volume, 
    // so every point of water we asume its volume is 1/N L 
    // so mass is 1/N kg;
    Point circle(1.0/2,3.0/5);
    //cout<<circle<<endl;
    double rsq=1.0/10;//initial water circle
    vector<double> ws(N+1, 0.0);
    for (int i=0;i<N;i++){
        //cout<<Ps[i];
        double ran=((double)rand()/(RAND_MAX));//add a few air particle into the water
        if (dist(Ps[i],circle)<=rsq&& ran>0.02) {
            mass[i]=200;
            ws[i]=1.0/double(N);
        }
    }
    vector<Polygon> Polys=SHDiagram(Ps,ws);
    save_svg_p(Polys,Ps,"init.svg");
    for (int i=0;i<iters;i++){
        OTweight(Ps,ws);
        Move(Ps,v,mass,ws);
        //cout<<Ps;
        vector<Polygon> Polys=SHDiagram(Ps,ws);
        save_svg_animated(Polys,mass,"fluid.svg", i, iters);
    }
}


#endif