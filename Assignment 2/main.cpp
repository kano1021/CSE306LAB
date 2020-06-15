#include <iostream>
#include <stdlib.h>
#include "SutherlandHodgman.h"
#include "kdtree.h"
#include "savesvg.h"
#include "voronoi2D.h"
#include "optimal.h"
#include "fluid.h"

using namespace std;

#define SIZE 200

//change SH to switch the method
#define SH 1
#define KD 1

bool polygoneq(Polygon a, Polygon b){
    for (int i=0;i<a.vertices.size();i++){
        if (!eq(a.vertices[i],b.vertices[i])) return false;
    }
    return true;
}

void testVoronoi(){
    vector<Point> sites;
    vector<double> ws;
    vector<vector<double> > siteskd;

    for (int i=0;i<SIZE;i++){
        Point x=Point(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
        double w=(double)(i+1)/SIZE/SIZE/10;
        sites.push_back(x);
        ws.push_back(w);
        //cout<<w<<endl;
    }
    double m=0.1/SIZE;
    for (int i=0;i<SIZE;i++){
        vector<double> p=Pmtovec(sites[i],ws[i],m);
        siteskd.push_back(p);
        //cout<<ws[i]<<" "<<p<<endl;
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
    save_svg_p(cells,sites, filename1);
    save_svg_p(cells_SH, sites, filename2);
}

void testoptimal(){
    vector<Point> sites;
    for (int i=0;i<SIZE;i++){
        Point x=Point(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
        sites.push_back(x);
    }

    //vector<double> Ws=OptimalTransport(sites,"normal");
    vector<double> Ws=OptimalTransport(sites,"uniform");

#if KD
    vector<vector<double> > siteskd;
    double maxw=0;
    for (auto &w: Ws){
        if (maxw<w) maxw=w;
    }
    //cout<<"m="<<maxw<<endl;
    for (int i=0;i<SIZE;i++){
        vector<double> p=Pmtovec(sites[i],Ws[i],maxw);
        siteskd.push_back(p);
        //cout<<p<<endl;
    }
    vector<Polygon> cells=voronoiDiagram(siteskd,maxw);
    //cout<<"KD="<<cells.size()<<endl;
    string filename1="opt_Kd.svg";

    save_svg_p(cells, sites, filename1);

#endif

#if SH
    string filename2="opt_SH.svg";

    vector<Polygon> cells_SH=SHDiagram(sites,Ws);
    //cout<<"SH="<<cells_SH.size()<<endl;
    save_svg_p(cells_SH, sites, filename2);
#endif
    for (int i; i<cells.size();i++){
        if (!(polygoneq(cells[i],cells_SH[i]))){
            cout<<"KD=";
            cells[i].print();
            cout<<"SH=";
            cells_SH[i].print();
            cout<<endl;
        }
    }

    return;
}

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

    
    //testoptimal();
    //Polygon outpolygon=SutherlandHodgman(subjectPolygon,clipPolygon);
    //outpolygon.print(); //some test
    //testKdTree();
    //testVoronoi();
    Fluid();
    return 0;
}