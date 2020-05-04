#ifndef SCENE_H
#define SCENE_H
#include<iostream>
#include "Sphere.h"
using namespace std;
class Scene {
public: 
    vector<Sphere*> spheres;
    Vector Light;
    double LightIntensity;

    Sphere* closest(const Ray& ray){
        double minn = INFINITY; 
        Sphere* closest_sphere;
        for (auto &sphere :spheres) {
            //cout<<sphere->R<<" ";
            Intersection its = sphere->intersect(ray);
            //if (sphere->R==10&&its.occured) cout<<"Yeah! "<< minn<<" "<<its.t<<endl;
            if (its.occured && its.t < minn) {
                //if (sphere->R==10) cout<<"Yeah! "<< minn<<" "<<its.t<<endl;
                minn = its.t;
                closest_sphere = sphere;
            }
        }
        //cout<<endl;
        return closest_sphere;
    }
    Scene(){
        Sphere* mul_red = new Sphere(Vector(0, 1000, 0), 940, Vector(1, 0, 0));
        Sphere* mul_green = new Sphere(Vector(0, 0, -1000), 940, Vector(0, 1, 0));
        Sphere* mul_blue = new Sphere(Vector(0, -1000, 0), 990, Vector(0, 0, 1));
        Sphere* mul_pink = new Sphere(Vector(0, 0, 1000), 940, Vector(1, 0, 1));
        Sphere* mul_yellow = new Sphere(Vector(1000, 0, 0), 940, Vector(1, 1, 0));
        Sphere* mul_cyan = new Sphere(Vector(-1000, 0, 0), 940, Vector(0, 1, 1));

        vector<Sphere*> muls;

        muls.push_back(mul_red);
        muls.push_back(mul_green);
        muls.push_back(mul_blue);
        muls.push_back(mul_pink);
        muls.push_back(mul_yellow);
        muls.push_back(mul_cyan);

        Vector Light=Vector(-10,20,40);
        this->spheres = muls;
        this->Light =Light;
        this->LightIntensity=100000;
    }
    Scene(vector<Sphere*> spheres, Vector Light, double LightIntensity=100000){
        this->spheres = spheres;
        this->Light =Light;
        this->LightIntensity=LightIntensity;
    }

    Intersection intersect(const Ray &ray) {
        Intersection its;
        //cout<<"Here 2"<<endl;
        Sphere* closest = this->closest(ray);
        //cout<<closest->R<<endl;
        if (closest->R == 0) {
            its.occured = false;
        } else {
            //cout<<"Here 5"<<endl;
            its = closest->intersect(ray);
            //cout<<"Here 6"<<endl;
            its.albedo = closest->albedo;
        }
        return its;
    }

    Vector getColor(const Ray &ray, int ray_depth) {
        if (ray_depth < 0) return Vector(0. , 0. , 0.) ; // terminates recursion at some ←􏰁 point
        Sphere *cst_s=this->closest(ray);
        Vector color(0.,0.,0.);
        Intersection its=this->intersect(ray);
        //cout<<"[" << its.P[0] << " "<< its.P[1] << " "<<its.P[2]  << " "<< its.t << " " <<"]"<<endl;
        if (its.occured) { 
            color=cst_s->albedo;
            //cout<<"[" << color[0] << " "<< color[1] << " "<<color[2] <<"]"<<endl;
            if (cst_s->mirror) {
                Ray reflected_ray = Ray(its.P, ray.u -its.N * (2 * dot(ray.u, its.N)));
                return this->getColor(reflected_ray, ray_depth - 1 );
            }else{
        // handle diffuse surfaces
                Vector S = this->Light;
                double I= this->LightIntensity;
                double d = norm(S - its.P);
                Vector w = normalize(S - its.P);
                Ray new_ray=Ray(S, -w);
    
                Intersection new_its = this->intersect(new_ray);
                bool visible=(!new_its.occured || new_its.t > d );
                //cout<<visible<<endl;
                //cout<<new_its.t<<" "<<d<<" "<<1/ M_PI * (I / (4 * M_PI * pow(d, 2))) * visible * max(dot(its.N, w), 0.)<<endl;
                color = (new_its.albedo / M_PI) * (I / (4 * M_PI * pow(d, 2))) * visible * max(dot(its.N, w), 0.);
            } 
        }
        return color;
    }

    void add(Sphere *sphere) {
        this->spheres.push_back(sphere); 
    }
};

#endif