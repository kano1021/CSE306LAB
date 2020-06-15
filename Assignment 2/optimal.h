#ifndef OBJ
#define OBJ
#include <stdio.h>
#include "lbfgs/lbfgs.c"
#include "gfunction.h"
#include "SutherlandHodgman.h"

using namespace std;

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
        int N=points.size();
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
        int ret = lbfgs(N, m_x, &fx, _evaluate,_progress, this, NULL);

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

        for (int i = 0;i < n; i ++) {
            Ws.push_back(x[i]);
            if (m<x[i]) m=x[i];
        }
        //USING KDTREE
#if 0
        vector<vector<double> > siteskd;
        
        for (int i=0;i<n; i++){
            vector<double> p=Pmtovec(points[i],Ws[i],m);
            siteskd.push_back(p);
        }
        vector<Polygon> cells=voronoiDiagram(siteskd,m);
#endif
        //USING SutherlandHodgman

        vector<Polygon> cells_SH=SHDiagram(points,Ws);

        for (int i=0 ; i<n ; i++){
            g[i]=- g_grad(cells_SH[i],lambdas[i],f);
            //cout<<g[i]<<", ";
        }
        fx= - g_func(cells_SH,points, Ws,lambdas,f);
        cout<<fx<<endl;
        return fx;
    }

    static int _progress(
        void *instance,
        const lbfgsfloatval_t *x,
        const lbfgsfloatval_t *g,
        const lbfgsfloatval_t fx,
        const lbfgsfloatval_t xnorm,
        const lbfgsfloatval_t gnorm,
        const lbfgsfloatval_t step,
        int n,
        int k,
        int ls
        )
    {
        return reinterpret_cast<objective_function*>(instance)->progress(x, g, fx, xnorm, gnorm, step, n, k, ls);
    }

    int progress(
        const lbfgsfloatval_t *x,
        const lbfgsfloatval_t *g,
        const lbfgsfloatval_t fx,
        const lbfgsfloatval_t xnorm,
        const lbfgsfloatval_t gnorm,
        const lbfgsfloatval_t step,
        int n,
        int k,
        int ls
        )
    {
        printf("Iteration %d:\n", k);
        printf("  fx = %f, x[0] = %f, x[1] = %f\n", fx, x[0], x[1]);
        printf("  xnorm = %f, gnorm = %f, step = %f\n", xnorm, gnorm, step);
        printf("\n");
        return 0;
    }
};


#endif