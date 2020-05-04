#ifndef IMAGE_H
#define IMAGE_H

#include<vector>
#include "Vector.h"
using namespace std;

struct Image {
    double W;   
    double H;   
    double fov;   
    vector<unsigned char> pixel;

    Image(double W, double H, double fov) {
        this->W = W;
        this->H = H;
        this->fov = fov;
        this->pixel = initpixel();
    }

    vector<unsigned char> initpixel() {
        vector<unsigned char> pixel(W * H * 3);
        return pixel;
    }
};

#endif