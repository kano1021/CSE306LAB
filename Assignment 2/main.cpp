#include <iostream>
#include <stdlib.h>
#include "SutherlandHodgman.h"
#include "kdtree.h"
#include "savesvg.h"
#include "voronoi2D.h"
#include "optimal.h"

using namespace std;

#define SIZE 2000


void testVoronoi(){
    vector<Point> sites;
    vector<double> ws;
    vector<vector<double> > siteskd;

    for (int i=0;i<SIZE;i++){
        Point x=Point(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
        double w=(double)0.1/SIZE;
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
    vector<double> ws;
    vector<double> prelambdas;
    Point ctr(0.5,0.5);
    double Sum=0;
    for (int i=0;i<SIZE;i++){
        Point x=Point(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
        double w=(double) 0.1;
        double lambda=exp(-dist(ctr,x)/0.02);//point nearer centre have larger area(lambda)
        Sum+=lambda;
        sites.push_back(x);
        ws.push_back(w);
        prelambdas.push_back(lambda);
    }

    //normalize lambdas to let there sum=1;
    vector<double> lambdas;
    for (int i=0;i<sites.size();i++) lambdas.push_back(prelambdas[i]/Sum);
    cout<<lambdas;
    objective_function obj;
    lbfgsfloatval_t *ret= obj.run(sites,ws,lambdas);
    vector<double> Ws;
    for (int i=0;i<SIZE;i++){
        Ws.push_back(ret[i]);
    }
    cout<<Ws;

#if 0
    vector<vector<double> > siteskd;
    double m=0;
    for (auto &w: Ws){
        if (m<w) m=w;
    }
    for (int i=0;i<sites.size();i++){
        vector<double> p=Pmtovec(sites[i],Ws[i],m);
        siteskd.push_back(p);
    }
    vector<Polygon> cells=voronoiDiagram(siteskd,m);

    string filename1="opt_Kd.svg";

    save_svg_p(cells, sites, filename1);

#endif

    string filename1="opt_SH.svg";

    vector<Polygon> cells_SH=SHDiagram(sites,Ws);
    save_svg_p(cells_SH, sites, filename1);
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

    
    testoptimal();
    //Polygon outpolygon=SutherlandHodgman(subjectPolygon,clipPolygon);
    //outpolygon.print(); //some test
    //testKdTree();
    //testVoronoi();
    return 0;
}