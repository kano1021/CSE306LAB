#ifndef OPTFLUID
#define OPTFLUID

#include <stdio.h>
#include "lbfgs/lbfgs.c"
#include "Nodesets.h"
#include "gfluid.h"
#include "SutherlandHodgman.h"

using namespace std;

//change SH to switch the method
#define SH 1
#define KD 0


class objective_function{
protected:
    lbfgsfloatval_t *m_x;
    vector<Point> points;
    vector<double> lambdas;
    double f;

public:
    objective_function() : m_x(NULL){}

    virtual ~objective_function(){
        if (m_x != NULL) {
            lbfgs_free(m_x);
            m_x = NULL;
        }
    }

    lbfgsfloatval_t* run(vector<Point> points, vector<double> ws, vector<double> lambdas,double f = 1){
        int N=points.size()+1;
        lbfgsfloatval_t fx;
        lbfgsfloatval_t *m_x = lbfgs_malloc(N);

        if (m_x == NULL) {
            printf("ERROR: Failed to allocate a memory block for variables.\n");
            return NULL;
        }

        /* Initialize the variables. */
        for (int i = 0;i < N;i ++) {
            m_x[i]=ws[i];
        }
        this->lambdas=lambdas;
        this->points=points;
        this->f=f;

        /*
            Start the L-BFGS optimization; this will invoke the callback functions
            evaluate() and progress() when necessary.
         */
        int ret = lbfgs(N, m_x, &fx, _evaluate, NULL, this, NULL);

        /* Report the result. */
        printf("L-BFGS optimization terminated with status code = %d\n", ret);
        printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, m_x[0], m_x[1]);
        
        return m_x;
    }

protected:
    static lbfgsfloatval_t _evaluate(
        void *instance,
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step
        )
    {
        return reinterpret_cast<objective_function*>(instance)->evaluate(x, g, n, step);
    }

    lbfgsfloatval_t evaluate(
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step
        )
    {
        lbfgsfloatval_t fx = 0.0;

        vector<double> Ws;
        
        double m=0;

        for (int i = 0;i < n-1; i ++) {
            Ws.push_back(x[i]);
            if (m<x[i]) m=x[i];
        }
        double w=x[n-1];
        //USING KDTREE
#if KD
        vector<vector<double> > siteskd;
        
        for (int i=0;i<n; i++){
            vector<double> p=Pmtovec(points[i],Ws[i],m);
            siteskd.push_back(p);
        }
        vector<Polygon> cells=voronoiDiagram(siteskd,m);
#endif
        //USING SutherlandHodgman
#if SH
        vector<Polygon> cells=SHDiagram(points,Ws);
#endif
        double areasum=0;
        for (int i=0 ; i<n-1 ; i++){
            if (lambdas[i]!=0)
                g[i]=- g_fluid_grad(cells[i],lambdas[i],f);
            else{
                g[i]=0;
                areasum+=Area(cells[i]);
            }
            //cout<<g[i]<<", ";
        }
        g[n-1]=lambdas[n-1]-areasum;

        fx= - g_fluid(cells,points, Ws,lambdas,w);

        //cout<<fx<<endl;
        return fx;
    }

};

void OTweight(vector<Point> X, vector<double> &ws){
    objective_function obj;
    int n=X.size();
    vector<double> lambdas(n+1,0.0);
    Point ctr(0.5,0.5);
    double sumi=0.0;
    for (int i=0;i<n;i++){
        if (ws[i]!=0)lambdas[i]=(1/double(n));
        else sumi+=1;
    }
    lambdas[n]=(sumi/double(n));
        //cout<<lambdas;
   
    lbfgsfloatval_t *w=obj.run(X, ws,lambdas);
    for (int i=0;i<n+1;i++){
        ws[i]=w[i];
    }
    //cout<<ws<<endl;
    return;
}


#endif