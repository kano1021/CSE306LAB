#ifndef KDTREE_H
#define KDTREE_H

#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<fstream>
#include "Polygon.h"
#include "Nodesets.h"

using namespace std;

int dimUsed;

bool cmp(vector<double> a, vector<double> b){
    if (a[dimUsed] < b[dimUsed])
        return true;
    return false;
}
 
class PwithD{
    public:
        double d;
        vector<double> p;
        PwithD(){}
        PwithD(vector<double> p,double d){
            this->p=p;
            this->d=d;
        }
}; 

class knear{
    public:
        double maxd;
        vector<PwithD> ps;
        knear(){}
        void add(vector<double> p, double d){
            PwithD pwd=PwithD(p,d);
            if (pwd.d>maxd || ps.size()==0){
                ps.push_back(pwd);
                maxd=pwd.d;
            }else if (ps.size()==1){
                ps.insert(ps.begin(),pwd);
            }else{
                int i=ps.size()-1;
                while ((ps[i-1].d>pwd.d)&&(i>0))i--;
                ps.insert(ps.begin()+i,pwd);
            }
        }
        void pop(){
            ps.pop_back();
            maxd=ps[ps.size()-1].d;
        }
        void popfront(){
            ps.erase(ps.begin());
        }
};
ostream & operator<<(ostream & os, knear ps)
{
    os << "[";
    for (auto &p: ps.ps) cout<<p.p<<", d="<<p.d<<"; ";
    os << "]";
    return os;
}

class KdTree{
    private:
        int d;
        int size;
        vector<vector<double> > data;
        Node * root;
    public:
        KdTree(vector<vector<double> > data, int dim){
            this->data=data;
            size=0;
            this->d=dim;
            root= createTreeNode(0,data.size()-1,0,size);
            size++;
        }

        Node* createTreeNode(int left, int right,int dim,int &idx){
            if (right < left){
                idx--;
                return NULL;
            }
            // rank with dim
            dimUsed = dim;
            sort(data.begin() + left, data.begin() + right+1, cmp);
            int mid = (left + right+1) / 2;
            Node * r=(Node *)malloc((1)*sizeof(Node));
            r = new Node(data[mid],dim,idx);
            r->l = createTreeNode(left, mid - 1, (dim + 1) % d, ++idx);
            if (r->l != NULL)
                r->l->p = r;
            r->r = createTreeNode(mid + 1, right, (dim + 1) % d, ++idx);
            if (r->r != NULL)
                r->r->p = r;
            return r;
        }

        void print(){
            printNode(root);
        }
        void printNode(Node* n){
            if (n==NULL) return;
            printNode(n->l);
            cout<<n->val<<" "<<n->idx<<", ";
            printNode(n->r);
        }

        Node* find(vector<double> d){
            int dim=0;
            double minDist=10000000;
            Node *r=root, *tmp;
            while(r!=NULL){
                tmp=r;
                if (d[dim] < r->val[dim])                           
                    r = r->l;   
                else
                    r = r->r;
                dim = (dim + 1) % this->d;
            }
            if (dist(d,tmp->val)>0){
                minDist=min(dist(d,tmp->val),minDist);
            }
            Node *nearNode=tmp;

            while (tmp->p!=NULL){
                tmp=tmp->p;
                double tmpd=dist(tmp->val,d);
                if (tmpd<minDist && tmpd>0){
                    nearNode=tmp;
                    minDist=tmpd;
                }
                Node *son;
                if (abs(tmp->val[tmp->axis] - d[tmp->axis]) < minDist){
                    if (tmp->val[tmp->axis] > d[tmp->axis])
                        son = tmp->r;
                    else
                        son = tmp->l;
                    FindinNode(d, minDist, nearNode, son);
                }
            }
            return nearNode;
        }
        
        void FindinNode(vector<double> d, double &minDist, Node* &nearNode, Node* tmp){
            if (tmp==NULL)
                return;
            double tmpd=dist(tmp->val,d);
            if (tmpd<minDist && tmpd>0){
                minDist=tmpd;
                nearNode=tmp;
            }
            
            if (abs(tmp->val[tmp->axis] - d[tmp->axis]) < minDist){
                FindinNode(d, minDist, nearNode, tmp->l);
                FindinNode(d, minDist, nearNode, tmp->r);
            }else{
                if (tmp->val[tmp->axis] > d[tmp->axis])
                    FindinNode(d, minDist, nearNode, tmp->l);
                else
                    FindinNode(d, minDist, nearNode, tmp->r);
            }
        }

        knear FindKnearest(vector<double> p, int k){
            knear retps;
            vector<bool> flg(data.size(),false);
            Node *r=root;
            Findnodes(p,r,k,flg,retps);
            return retps;
        }
        void Findnodes(vector<double> p, Node *r, int k, vector<bool> &flg, knear &retps){
            if (r==NULL) return;
            Node * tmp=NULL;
            while(r!=NULL){
                tmp=r;
                if (r->l==NULL){
                    r=r->r;
                }else if (r->r==NULL){
                    r=r->l;
                }else if (p[r->axis] < r->val[r->axis])                           
                    r = r->l;   
                else
                    r = r->r;
            }
            //cout<<"point"<<tmp->idx<<" "<<tmp->val<<" "<<retps<<endl; 
            if ((!eq(p,tmp->val))&&(!flg[tmp->idx])){
                double d=dist(tmp->val,p);
                if (retps.ps.size()<k) retps.add(tmp->val,d);
                else if (d<retps.maxd){
                    retps.pop();
                    //cout<<"alt:"<<tmp->idx<<" "<<tmp->val<<" "<<retps<<endl; 
                    retps.add(tmp->val,d);
                }
            }

            flg[tmp->idx]=true;

            while (tmp->p!=NULL){
                tmp=tmp->p;
                //cout<<"point"<<tmp->idx<<" "<<tmp->val<<" "<<retps<<endl; 
                while (flg[tmp->idx] && tmp->p!=NULL) tmp=tmp->p;

                if (tmp->idx==0 && flg[0]) return;

                flg[tmp->idx]=true;
                double d=dist(tmp->val,p);
                if (!eq(p,tmp->val)){
                    if (retps.ps.size()<k ){
                        retps.add(tmp->val,d);   
                        //cout<<"add:"<<tmp->idx<<" "<<tmp->val<<" "<<retps<<endl; 
                    }else if (d<retps.maxd){
                        retps.pop();
                        //cout<<"alt:"<<tmp->idx<<" "<<tmp->val<<" "<<retps<<endl; 
                        retps.add(tmp->val,d);
                    }
                }
                
                if (abs(tmp->val[tmp->axis] - p[tmp->axis])<retps.maxd||retps.ps.size()<k){
                    //cout<<"<-down"<<endl;
                    Findnodes(p,tmp->l,k,flg,retps);
                    Findnodes(p,tmp->r,k,flg,retps);
                }else{
                    if (tmp->val[tmp->axis] > p[tmp->axis]) 
                        Findnodes(p,tmp->r,k,flg,retps);
                    else 
                        Findnodes(p,tmp->l,k,flg,retps);
                }
            }
            return;
        }
};

bool loadData(string filename, vector<vector<double> > &data){
    ifstream infs(filename);
    if (infs.is_open()){
        int num,dim;
        infs >> num>>dim;
        for (int i = 0; i < num; i++){
            vector<double> x;
            for (int j =0;j<dim;j++){
                double y;
                infs>>y;
                x.push_back(y);
            }
            data.push_back(x);
        }
        return true;
    }
    return false;
}


void testKdTree(){
    vector<vector<double> > data;
    loadData("kd.txt",data);
    KdTree* kdtree=new KdTree(data,data[0].size());
    kdtree->print();
    cout<<endl;
    vector<double> testdata;
    testdata.push_back(4);
    testdata.push_back(5);
    knear r=kdtree->FindKnearest(testdata,5);
    for (auto& p: r.ps){
        cout<<p.p<<" d="<<p.d<<" ";
    }
}

#endif