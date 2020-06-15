#ifndef VORO
#define VORO

#include <iostream>
#include <string>
#include <cmath>
#include "kdtree.h"
#include "Nodesets.h"
#include "Polygon.h"
#include "savesvg.h"
using namespace std;


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
            if (outPolygon.vertices.size()==0)return outPolygon;
            subjectPolygon=outPolygon;
            //subjectPolygon.print();  //some test
        }
    }
    return subjectPolygon;
}

double farest(Point p,Polygon points){
    double l=0;
    for (auto& x: points.vertices){
        double d=dist(x,p);
        if(l<d) l=d;
    }
    return l;
}

Polygon voronoicell(vector<double> P, vector<vector<double> > points, double m){
    Point PP=vectoP(P);
    double w;
    w=m-P[2]*P[2];
    //cout<<"m="<<m<<", w="<<w<<"P="<<P[2]*P[2]<<endl;
    vector<Point> square;
    square.push_back(Point(0,0));
    square.push_back(Point(0,1));
    square.push_back(Point(1,1));
    square.push_back(Point(1,0));
    Polygon subjectPolygon(square);
    int startk=0,k=int(log(points.size())/log(2));
    //cout<<"here!"<<endl;
    KdTree* kdtree=new KdTree(points,points[0].size());
    knear r=kdtree->FindKnearest(P,k);
    double farst=(farest(PP,subjectPolygon));
    //cout<<"here!"<<endl;
    while (r.ps[r.ps.size()-1].d<farst && startk<k){
        vector<vector<double> > knn; 
        //cout<<"here!"<<startk<<","<<k<<endl;
        for (int i=startk;i<k;i++) {
            //cout<<r.ps[i].p<<endl;
            knn.push_back(r.ps[i].p);
        }
        subjectPolygon=clipp(PP,w,knn,subjectPolygon,m);
        startk=k;
        k=2*k;
        if (k>points.size()-1) k=points.size()-1;
        
        r=kdtree->FindKnearest(P,k);
        
        farst=(farest(PP,subjectPolygon));
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
        //cout<<points[i]<<endl;
        Polygon cell=voronoicell(points[i],points,m);
        cells.push_back(cell);
    }
    return cells;
}

#endif