#include <iostream>
#include <vector>
#include "./../src/01-test.h"

using namespace std;
double xn;
double r;
double get_logistic(){
    xn=xn*r*(1.0-xn);
    return xn;
}
int main(){
    r=3.0; //control parameter
    xn=0.7; //initial condition
    vector<double> dat;
    for (int i=0; i<3000; ++i){
        dat.push_back(get_logistic()); // get data from logistic map
    }

    test01 test;
    test.init(dat);
    test.make_test();
    double k=test.get_value();
    //cout<<"01-test: "<<k<<endl;
    test.print_pcqc();
    return 0;
}