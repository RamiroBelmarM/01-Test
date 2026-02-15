#include <fstream>
#include "./../src/01-test.h"

double get_logistic(double r, double xn){
    xn=xn*r*(1.0-xn);
    return xn;
}
double get_random(double min, double max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}
int main(){
    std::ofstream archivo("dat.dat", std::ios::out | std::ios::trunc);

    std::vector<double> dat_logistic;
    std::vector<double> dat_logistic_noise;

    double r;
    double xn;
    double xn_noise;
    Test01 test;
    for (int i=0; i<1000; i++ ){
        r=get_random(3.5,3.99);
        archivo<<r<<'\t';
        std::cout<<i<<"\t"<<r<<std::endl;
        xn=get_random(0.01,0.99);
        xn_noise=xn+get_random(-0.25,0.25);

        for (int j=0; j<2100; j++ ){
            xn=get_logistic(r,xn);
            xn_noise=xn + 0.1*get_random(-1,1); //10% noise
            if (j>1000){
                dat_logistic.push_back(xn);
                dat_logistic_noise.push_back(xn_noise);
            }
        }

        test.init(dat_logistic,false);
        test.make_test();
        archivo<<test.get_value()<<'\t';
        test.clear();

        test.init(dat_logistic_noise, true);
        test.make_test();
        archivo<<test.get_value()<<std::endl;
        test.clear();

        std::vector <double>().swap(dat_logistic);
        std::vector <double>().swap(dat_logistic_noise);
    }
    archivo.close();
    
    return 0;
}