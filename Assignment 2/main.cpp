#include <iostream>
#include <stdlib.h>
#include "SutherlandHodgman.h"
#include "kdtree.h"
#include "savesvg.h"
#include "voronoi2D.h"

using namespace std;

#define SIZE 50


int main(){
    vector<Point> square;
    square.push_back(Point(0,0));
    square.push_back(Point(0,4));
    square.push_back(Point(4,4));
    square.push_back(Point(4,0));
    Polygon subjectPolygon(square);
    vector<Point> diamond;
    diamond.push_back(Point(-1,2));
    diamond.push_back(Point(2,5));
    diamond.push_back(Point(5,2));
    diamond.push_back(Point(2,-1));
    Polygon clipPolygon(diamond);

    vector<Point> sites;
    vector<double> ws;
    vector<vector<double> > siteskd;

    double m=0;
    for (int i=0;i<SIZE;i++){
        Point x=Point(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
        double w=(double) rand() / (RAND_MAX)/SIZE/10;
        if (m<w) m=w;
        cout<<x;
        sites.push_back(x);
        ws.push_back(w);
    }
    for (int i=0;i<SIZE;i++){
        vector<double> p=Pmtovec(sites[i],ws[i],m);
        siteskd.push_back(p);
    }
    //cout<<endl;

    vector<Point> s2;
    //cout<<Point(0.25,0.25);
    s2.push_back(Point(0.25,0.25));
    s2.push_back(Point(0.25,0.75));
    s2.push_back(Point(0.75,0.75));
    s2.push_back(Point(0.75,0.25));
    s2.push_back(Point(0.4,0.4));

    vector<Polygon> cells,cells_SH;
    cells_SH=SHDiagram(sites,ws);
    cells=voronoiDiagram(siteskd,m);
    string filename1="voronoi_Kd.svg",filename2="SH.svg";
    save_svg(cells, filename1);
    save_svg(cells_SH, filename2);


    //Polygon outpolygon=SutherlandHodgman(subjectPolygon,clipPolygon);
    //outpolygon.print(); //some test
    //testKdTree();
}