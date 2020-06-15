#ifndef LLOYD_H
#define LLOYD_H

#include "voronoi2D.h"
#include "SutherlandHodgman.h"

vector<Point> lloyd(int SIZE, int maxit = 5) {
    vector<Point> points;
    for (int i=0;i<SIZE;i++){
        Point x=Point(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
        points.push_back(x);
    }
    vector<double> ws(SIZE, 1/SIZE);
    for (int it=0;it < maxit;it++) {
        vector<Polygon> cells = SHDiagram(points,ws);
        //save_svg_animated(cells,"lloyd.svg", it, maxit);
        #pragma omp parallel for schedule(dynamic, 1)
        for (int i = 0; i < int(points.size()); i++) {
            points[i] = centroid(cells[i]);
        }
    }
    return points;
}

#endif