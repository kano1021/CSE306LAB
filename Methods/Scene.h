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
    bool inLight;

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
    Scene(bool inLight=false){
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
        this->inLight=inLight;
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
            Vector P=its.P+its.N*1e-4;
            //cout<<"[" << color[0] << " "<< color[1] << " "<<color[2] <<"]"<<endl;
            if (cst_s->type==0) {
                Ray reflected_ray = Ray(P, ray.u -its.N * (2 * dot(ray.u, its.N)));
                return this->getColor(reflected_ray, ray_depth - 1 );
            }
            
            if (cst_s->type==-1){
        // handle diffuse surfaces
                Vector S = this->Light;
                double I= this->LightIntensity;
                double d = norm(S - P);
                Vector w = normalize(S - P);

                Ray new_ray=Ray(S, -w);
    
                Intersection new_its = this->intersect(new_ray);
                bool visible=(!new_its.occured || new_its.t > d );
                //cout<<visible<<endl;
                //cout<<new_its.t<<" "<<d<<" "<<1/ M_PI * (I / (4 * M_PI * pow(d, 2))) * visible * max(dot(its.N, w), 0.)<<endl;
                color = (new_its.albedo / M_PI) * (I / (4 * M_PI * pow(d, 2))) * visible * max(dot(its.N, w), 0.);
                return color;
            } 
            if (cst_s->type>0){
                double n=cst_s->type;
                //cout<<n<<" ";
                Vector N=its.N;
                Vector w=ray.u;
                P=its.P-N*1e-4;
                double k0 = pow(((1 - n)/(1 + n)), 2);
                double R = k0 + (1 - k0) * pow((1 - abs(dot(N, w))), 5);
                double u = (double) rand() / (RAND_MAX);

                if (u<R){
                    Ray reflected_ray = Ray(P, w -N * (2 * dot(w, N)));
                    color= this->getColor(reflected_ray, ray_depth - 1 );
                }else{
                    double g=dot(w,N);
                    if (g>0)cout<<g<<endl;
                    
                    //cout<<n<<" w:["<<w[0]<<","<<w[1]<<","<<w[2]<<" N:"<<"["<<N[0]<<","<<N[1]<<","<<N[2]<<" "<<endl;
                    if (dot(w,N)>0) {
                        n=1/n;
                        //cout<<n<<endl;
                        P=its.P+N*1e-4; 
                        N=-N;
                    }
                    Vector wt=(w-N*dot(w,N))/n;
                    Vector wn(0.,0.,0.);
                    double t=1-(1-pow(dot(w,N),2))/pow(n,2);
                    if (t>0) wn=-N*sqrt(t);
                    Ray new_ray(P,wt+wn);
                    color=this->getColor(new_ray, ray_depth - 1 ); 
                }
            }
            if (this->inLight){

            }
        }
        return color;
    }

    void add(Sphere *sphere) {
        this->spheres.push_back(sphere); 
    }
};

#endif