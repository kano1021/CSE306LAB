#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Library/stb_image_write.h"
#include "Methods/Scene.h"
#include <iostream>

using namespace std;
double min(double a,double b){
    if (a<b) return a;
    else return b;
}
int main() {
// first define the scene , variables , ... // then scan all pixels
    int W = 512;
    int H = 512;
    double fov = 350;
    int NB_path = 10;
    Scene scene;
    Sphere *ball1=new Sphere(Vector(0, 0, 0), 10, Vector(1, 1, 1));
    scene.add(ball1);
    Vector camera(0,0,55);
    Image image=Image(W,H, fov);
    for (int i=0; i<H; i++) {
        for (int j=0; j<W; j++) {
            Vector V_O(j + 0.5 - W/2, i + 0.5 - H/2, 55 - fov);
            Vector V_u=normalize(V_O - camera);
            Ray ray(V_O, V_u); // cast a ray from the camera center to pixel i , j
            Vector color(0.,0.,0.);
            for (int k=0;k<NB_path;k++){
                color = color+ scene.getColor(ray, 2) ;
            }
            color/=NB_path;
            cout<<"[" << color[0] << " "<< color[1] << " "<<color[2] <<"]"<<endl;
            image.pixel[i*W*3+j*3 + 0] = min(255, pow(color[0] , 1./2.2)*255); // stores R ←􏰀channel
            // same for green and blue
            image.pixel[i*W*3+j*3 + 1] = min(255, pow(color[1] , 1./2.2)*255); // stores G ←􏰀channel
            image.pixel[i*W*3+j*3 + 2] = min(255, pow(color[2] , 1./2.2)*255); // stores B ←􏰀channel
            } 
        }
// save image 
    stbi_write_png("lab1.png", W, H, 3, &image.pixel[0], 0);
    
    return 0;
}