#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Library/stb_image_write.h"
#include "Methods/Scene.h"
#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;

int main() {
// first define the scene , variables , ... // then scan all pixels
    int W = 512;
    int H = 512;
    double fov = M_PI/3;
    int NB_path = 10;
    Scene scene(true);
    Sphere *ball1=new Sphere(Vector(-20., 0., 0.), 10., Vector(1., 1., 1.),0);
    Sphere *ball2=new Sphere(Vector(0., 0., 0.), 10., Vector(1., 1., 1.),1.3);
    Sphere *ball3=new Sphere(Vector(20., 0., 0.), 10., Vector(1., 1., 1.),1.7);
    scene.add(ball1);
    scene.add(ball2);
    scene.add(ball3); 
    //cout<<scene.spheres.size()<<endl;
    Vector camera(0,0,55);
    Image image=Image(W,H, fov);
    //cout<<"Here 1"<<endl;
    #pragma omp parallel for
    for (int i=0; i<H; i++) {
        for (int j=0; j<W; j++) {
            double x, y;
            Vector V_O(j + 0.5 - W/2, i + 0.5 - H/2, 55 - W / (2 * tan(fov/ 2)));
            Vector V_u=normalize(V_O - camera);
            Ray ray(camera, V_u); // cast a ray from the camera center to pixel i , j
            Vector color(0.,0.,0.);
            //cout<<"Here 2"<<endl;
            for (int k=0;k<NB_path;k++){
                boxMuller(1, x, y);
                color = color+ scene.getColor(ray,5);
            }
            color/=NB_path;
            image.pixel[(H-i-1)*W*3+j*3 + 0] = max(0.,min(255., pow(color[0] , 1./2.2)*255)); // stores R ←􏰀channel
            // same for green and blue
            image.pixel[(H-i-1)*W*3+j*3 + 1] = max(0.,min(255., pow(color[1] , 1./2.2)*255)); // stores G ←􏰀channel
            image.pixel[(H-i-1)*W*3+j*3 + 2] = max(0., min(255., pow(color[2] , 1./2.2)*255)); // stores B ←􏰀channel
            } 
        }
// save image 
    stbi_write_png("triball without anti.png", W, H, 3, &image.pixel[0], 0);
    
    return 0;
}