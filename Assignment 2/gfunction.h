#ifndef GF
#define GF
#include "voronoi2D.h"

using namespace std;

double Area(Polygon polygon) {
    double area = 0; 
    int n = int(polygon.vertices.size());
    for (int i = 0; i < n; i++) {
        area += (polygon.vertices[(i+n-1)%n].x + polygon.vertices[i].x) * 
                (polygon.vertices[(i+n-1)%n].y - polygon.vertices[i].y); 
    } 
    return abs(area / 2); 
}

double integral(Polygon cell, Point P) {
    double sum = 0;
    for (int k = 1; k < cell.vertices.size(); k ++) {
        double a = cell.vertices[k].x;
        double b = cell.vertices[k].y;
        double x = cell.vertices[k-1].x;
        double y = cell.vertices[k-1].y;
        sum += (a * b - a * b) * (x*x + x*a + a*a + y*y + y*b + b*b
                - 4 * (P.x*(x + a) + P.y*(y + b)) + 6 * P.sq2());
    }
    return sum/12;
}

double g_func(vector<Point>& points, vector<double>& ws, double f = 1) {
    // function g
    double sum = 0;
    vector<vector<double> > siteskd;
    double m=0;
    for (auto &w: ws){
        if (m<w) m=w;
    }
    for (int i=0;i<points.size();i++){
        vector<double> p=Pmtovec(points[i],ws[i],m);
        siteskd.push_back(p);
    }
    vector<Polygon> cells=voronoiDiagram(siteskd,m);
    for (int i = 1; i < int(points.size()); i ++) {
        double area = Area(cells[i]);
        sum += f * (integral(cells[i], points[i]) - s[i] * area) + area * ws[i];
    }
    return sum;
}

double g_grad(vector<Point>& points, int i, vector<double>& ws, double f = 1) {
    vector<vector<double> > siteskd;
    double m=0;
    for (auto &w: ws){
        if (m<w) m=w;
    }
    for (int i=0;i<points.size();i++){
        vector<double> p=Pmtovec(points[i],ws[i],m);
        siteskd.push_back(p);
    }
    Polygon cell = voronoicell(siteskd[i], siteskd, m);
    double area = Area(cell);
    return - f * area + area;
}

#endif