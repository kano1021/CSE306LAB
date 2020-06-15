#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;


class Point{
    public:
        double x,y;
        Point(){}
        Point(double x, double y){
            this->x=x;
            this->y=y;
        }
        double sq2(){
            return x*x+y*y;
        }
};

Point operator-(const Point& a, const Point& b) {
    return Point(a.x- b.x, a.y-b.y);
}
Point operator+(const Point& a, const Point& b) {
    return Point(a.x+ b.x, a.y+b.y);
}
Point operator*(const Point& a, const double& b) {
    return Point(a.x*b, a.y*b);
}
Point operator/(const Point& a, const double& b) {
    return Point(a.x/b, a.y/b);
}

double dist(Point a, Point b){
    return Point(a-b).sq2();
}

double dot(Point a, Point b){
    return a.x*b.x+a.y*b.y;
}

bool eq(Point a, Point b){
    return a.x==b.x && a.y==b.y;
}

ostream & operator<<(ostream & os, Point vi)
{
    os << "(";
    cout << vi.x << ","<<vi.y;
    os << ")";
    return os;
}

class Line{
    public:
        Point s,e;//start, end
        Line(Point a, Point b){
            this->s=a;
            this->e=b;
        }
        Line(double x1,double y1,double x2,double y2){
            this->s=Point(x1,y1);
            this->e=Point(x2,y2);
        }
};

double side(Line l, Point p){
	return (l.e.x - l.s.x) * (p.y - l.s.y) - (l.e.y - l.s.y) * (p.x - l.s.x);
}

Point intersect(Line a, Line b){
    double x=((a.s.x*a.e.y-a.s.y*a.e.x)*(b.s.x-b.e.x)-(b.s.x*b.e.y-b.s.y*b.e.x)*(a.s.x-a.e.x))/((a.s.x-a.e.x)*(b.s.y-b.e.y)-(a.s.y-a.e.y)*(b.s.x-b.e.x));
    double y=((a.s.x*a.e.y-a.s.y*a.e.x)*(b.s.y-b.e.y)-(b.s.x*b.e.y-b.s.y*b.e.x)*(a.s.y-a.e.y))/((a.s.x-a.e.x)*(b.s.y-b.e.y)-(a.s.y-a.e.y)*(b.s.x-b.e.x));
    return Point(x,y);
}

Point internorm(Point A, Point B, Point ps, Point pe, double wi, double wj){
    Point M=(ps+pe)/2;
    //cout<<wi<<wj<<"M="<<M;
    M=M+(pe-ps)*(wi-wj)/(2*(pe-ps).sq2());
    //cout<<", after M="<<M;
    double t=dot(M-A,ps-pe)/dot(B-A,ps-pe);
    //cout<<"M="<<M<<",t="<<t<<endl;
    return A+((B-A)*t);
}

double side2(Line l, Point p, double wi, double wj){
    Point M=(l.s+l.e)/2;
    M=M+(l.e-l.s)*(wi-wj)/(2*(l.e-l.s).sq2());
	return dot(p-M,l.e-l.s);
}

class Polygon{
    public:
        vector<Point> vertices;
        Polygon(){};
        Polygon(vector<Point> vertices){
            this->vertices=vertices;
        }
        void print(){
            for (auto &point: this->vertices){
                cout<<"("<<point.x<<", "<<point.y<<") ";
            }
            cout<<endl;
        }
};

Point centroid(Polygon ps){
    Point r;
    for (auto& p: ps.vertices){
        r=r+p;
    }
    r=r/ps.vertices.size();
    return r;
}


#endif