#ifndef SH_H
#define SH_H
#include <iostream>
#include <vector>
#include "Polygon.h"
using namespace std;

Polygon SutherlandHodgman(Polygon subjectPolygon, Polygon clipPolygon){
    bool prevside=false;
    Point sp=clipPolygon.vertices[clipPolygon.vertices.size()-1];
    for (auto &clippoint: clipPolygon.vertices){// For each edge of the clip polygon
        // Clip the subjectPolygon by a half-space
        Polygon outPolygon;
        Point prevVertex =subjectPolygon.vertices[subjectPolygon.vertices.size()-1];
        prevside=(side(Line(sp,clippoint),prevVertex)<=0);
        for (int i=0; i<subjectPolygon.vertices.size();++i){// For each vertex of the subject polygon
            // Test the subject polygon edge with vertices (i-1, i)
            Point curVertex = subjectPolygon.vertices[i];
            // Compute inter. between the infinite line supported by clipEdge and edge (i-1, i) Vector intersection = intersect(prevVertex, curVertex, clipEdge);
            if (side(Line(sp,clippoint),curVertex)<=0){
                if (!prevside){
                    prevside=true;
                    Point it=intersect(Line(sp,clippoint),Line(prevVertex,curVertex));
                    outPolygon.vertices.push_back(it);
                }
                outPolygon.vertices.push_back(curVertex);
            }else if (prevside){
                prevside=false;
                Point it=intersect(Line(sp,clippoint),Line(prevVertex,curVertex));
                outPolygon.vertices.push_back(it);
            }
            prevVertex = curVertex;
        }
        sp=clippoint;
        subjectPolygon=outPolygon;
        //subjectPolygon.print();  //some test
    }
    return subjectPolygon;
}

Polygon SH2(Point P, double w, vector<Point> points, vector<double> ws){
    bool prevside=false;
    vector<Point> square;
    square.push_back(Point(0,0));
    square.push_back(Point(0,1));
    square.push_back(Point(1,1));
    square.push_back(Point(1,0));
    Polygon subjectPolygon(square);
    for (int j=0;j<points.size();j++){// For each edge of the clip polygon
        // Clip the subjectPolygon by a half-space
        Point ep=points[j];
        double wj=ws[j];
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

vector<Polygon> SHDiagram(vector<Point> sites,vector<double> ws){
    vector<Polygon> cells_SH;
    for (int i=0;i<sites.size();i++){
        Polygon cell2=SH2(sites[i],ws[i],sites,ws);
        cells_SH.push_back(cell2);
    }
    return cells_SH;
}

#endif
