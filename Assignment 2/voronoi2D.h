#ifndef VORO
#define VORO

#include <iostream>
#include "kdtree.h"
#include "Nodesets.h"
#include "Polygon.h"
using namespace std;

vector<Point> site;

vector<double> interweight(vector<double> A, vector<double> B, vector<double> ps, vector<double> pe, double wi, double wj){
    vector<double> M=(ps+pe)/2;
    vector<double> p=pe-ps;
    M=M+p/(2*sq2(p))*(wi-wj);
    double t=dot(M-A,p)/dot(B-A,p);
    return A+((B-A)*t);
}

Point vectoP(vector<double> P){//transform vector to Point
    return Point(P[0],P[1]);
}

vector<double> Pmtovec(Point P, double w, double m){
    vector<double> r;
    r.push_back(P.x);
    r.push_back(P.y);
    r.push_back(sqrt(m-w));
    return r;
}

Polygon clipp(Point P, double w, vector<vector<double> > points,  Polygon subjectPolygon,double m){
    bool prevside=false;
    for (auto &p:points){// For each edge of the clip polygon
        // Clip the subjectPolygon by a half-space
        Point ep=vectoP(p);
        double wj=m-p[2]*p[2];
        if (!eq(ep,P)){
            Polygon outPolygon;
            Point prevVertex =subjectPolygon.vertices[subjectPolygon.vertices.size()-1];
            prevside=(side2(Line(P,ep),prevVertex,w,wj)<0);
            for (int i=0; i<subjectPolygon.vertices.size();++i){// For each vertex of the subject polygon
                // Test the subject polygon edge with vertices (i-1, i)
                Point curVertex = subjectPolygon.vertices[i];
                // Compute inter. between the infinite line supported by clipEdge and edge (i-1, i) Vector intersection = intersect(prevVertex, curVertex, clipEdge);
                if (side2(Line(P,ep),curVertex,w,wj)<0){
                    if (!prevside){
                        prevside=true;
                        Point it=internorm(prevVertex,curVertex,P,ep,w,wj);
                        
                        outPolygon.vertices.push_back(it);
                    }
                    outPolygon.vertices.push_back(curVertex);
                   
                }else if (prevside){
                    prevside=false;
                    Point it=internorm(prevVertex,curVertex,P,ep,w,wj);
                    
                    outPolygon.vertices.push_back(it);
                }
                prevVertex = curVertex;
            }
            
            subjectPolygon=outPolygon;
            //subjectPolygon.print();  //some test
        }
    }
    return subjectPolygon;
}

double farest(Point p, double w,Polygon points){
    double l=0;
    for (auto& x: points.vertices){
        double d=dist(x,p)-w;
        if(l<d) l=d;
    }
    return l;
}

Polygon voronoicell(vector<double> P, vector<vector<double> > points, double m){
    Point PP=vectoP(P);
    double w=m-P[2]*P[2];
    vector<Point> square;
    square.push_back(Point(0,0));
    square.push_back(Point(0,1));
    square.push_back(Point(1,1));
    square.push_back(Point(1,0));
    Polygon subjectPolygon(square);
    int startk=0,k=4;
    KdTree* kdtree=new KdTree(points,points[0].size());
    knear r=kdtree->FindKnearest(P,k);
    double farst=(farest(PP,w,subjectPolygon))*4;
    while (r.ps[r.ps.size()-1].d<farst){
        vector<vector<double> > knn; 
        for (int i=startk;i<k;i++) knn.push_back(r.ps[i].p);
        subjectPolygon=clipp(PP,w,knn,subjectPolygon,m);
        startk=k;
        k=k+k;
        if (k>points.size()) k=points.size();
        r=kdtree->FindKnearest(P,k);
        farst=(farest(PP,w,subjectPolygon))*4;
    }
    vector<vector<double> > knn;
    for (int i=startk;i<k;i++) {
        if (r.ps[i].d<farst)
            knn.push_back(r.ps[i].p);
        else
            break;
    }
    subjectPolygon=clipp(PP,w,knn,subjectPolygon,m);
    return subjectPolygon;
}

vector<Polygon> voronoiDiagram(vector<vector<double> > points, double m){
    vector<Polygon> cells;
    for (int i=0;i<points.size();i++){
        Polygon cell=voronoicell(points[i],points,m);
        cells.push_back(cell);
    }
    return cells;
}

#endif