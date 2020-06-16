#ifndef GFLUID
#define GFLUID
#include "polygon.h"

using namespace std;

double Area(Polygon polygon) {
    double area = 0; 
    int n = int(polygon.vertices.size());
    for (int i = 0; i < n; i++) {
        area += polygon.vertices[(i+n-1)%n].y * polygon.vertices[i].x - 
                polygon.vertices[(i+n-1)%n].x * polygon.vertices[i].y; 
    } 
    return abs(area / 2); 
}

double integral(Polygon cell, Point P) {
    double sum = 0;
    int N=cell.vertices.size();
    for (int k = 0; k < N; k ++) {
        double a = cell.vertices[(k+1)%N].x;
        double b = cell.vertices[(k+1)%N].y;
        double x = cell.vertices[k].x;
        double y = cell.vertices[k].y;
        sum += (x * b - a * y) * (x*x + x*a + a*a + y*y + y*b + b*b
                - 4 * (P.x*(x + a) + P.y*(y + b)) + 6 * P.sq2());
    }
    return sum/12;
}

double g_fluid(vector<Polygon> cells, vector<Point> points, vector<double> ws,
                vector<double> lambdas, double w) {
    // function g for fluid
    double sum = 0, sumarea= 0;
    
    for (int i = 1; i < int(points.size()); i ++) {
        double area = Area(cells[i]);
        sum +=  integral(cells[i], points[i]);
        if (ws[i]!=0){
            sum +=  - ws[i] * area + lambdas[i] * ws[i];
        }else{
            sumarea+=area;
        }
    }
    sum +=  (lambdas[points.size()]-sumarea) * w;
    return sum;
}

double g_fluid_grad(Polygon cell, double lambda, double f = 1) {
    double area = Area(cell);
    return - f * area + lambda;
}

#endif