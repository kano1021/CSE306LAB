#ifndef NODE_H
#define NODE_H
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>

using namespace std;

class Node{
	public:
		int axis;
        int idx;
		Node *p, *l, *r;//parent, left, right.
        vector<double> val;
		Node() {}
        Node(vector<double> val, int ax,int i){
            this->val=val;
            this->axis=ax;
            this->idx=i;
            this->p=NULL;
            this->l=NULL;
            this->r=NULL;
        }
}; 

vector<double> operator-(const vector<double>& a, const vector<double>& b) {
    vector<double> p;
    for (int i=0;i<a.size();i++){
        p.push_back(a[i]-b[i]);
    }
    return p;
}
vector<double> operator+(const vector<double>& a, const vector<double>& b) {
    vector<double> p;
    for (int i=0;i<a.size();i++){
        p.push_back(a[i]+b[i]);
    }
    return p;
}
vector<double> operator*(const vector<double>& a, const double& b) {
    vector<double> p;
    for (int i=0;i<a.size();i++){
        p.push_back(a[i]*b);
    }
    return p;
}
vector<double> operator/(const vector<double>& a, const double& b) {
    vector<double> p;
    for (int i=0;i<a.size();i++){
        p.push_back(a[i]/b);
    }
    return p;
}

double dist(vector<double> a, vector<double> b){
    double s=0;
    for (int i=0;i<a.size();i++){
        s+=(a[i]-b[i])*(a[i]-b[i]);
    }
    return s;
}

double dot(vector<double> a, vector<double> b){
    double s=0;
    for (int i=0;i<a.size();i++){
        s+=a[i]*b[i];
    }
    return s;
}

double sq2(vector<double> a){
    double s=0;
    for (int i=0;i<a.size();i++){
        s+=a[i]*a[i];
    }
    return s;
}

bool eq(vector<double> a, vector<double> b){
    for (int i=0;i<a.size();i++){
        if (a[i]!=b[i]) return false;
    }
    return true;
}

ostream & operator<<(ostream & os, vector<double> vi){
    os << "(";
    for (int i = 0; i < vi.size(); i++)
        cout << vi[i] << ",";
    os << ")";
    return os;
}


#endif