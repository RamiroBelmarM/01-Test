#ifndef __01TEST__
#define __01TEST__
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <vector>
#include <stdexcept>
class Test01{
    private:
        const double my_pi = 3.14159265358979323846;
        std::vector <double> dat;
        std::vector <double> dat_sum;
        std::vector <double> qc_vector;
        std::vector <double> pc_vector;
        std::vector <double> mc;
        std::vector <double> kc;
        std::vector <double> xi;
        double kc_final_value; //valor final
        double MSD_cutoff; //
        unsigned int  mcIterations = 100; //numero de pruebas Monte Carlo
        double deltaT = 0.1; //tiempo de muestreo 
        int min_length=1000; //Tamaño mínimo de las muestras
        bool noise;
    public:
        void init(std::vector<double> input, bool flag){
            std::vector<double>().swap(dat);
            dat=input;
            noise=flag;
        }
        void init(std::vector<double> input, double dt, bool flag){
            std::vector<double>().swap(dat);
            dat=input;
            deltaT=dt;
            noise=flag;
        }
        void make_test(){
            if (dat.size()<min_length){
                std::cerr<<"Error, los datos deben tener al menos 1000 observaciones"<<std::endl;
                kc_final_value = std::numeric_limits<double>::quiet_NaN();
                return;
            }
            make_sum(); 
            std::vector<double>().swap(mc);
            std::vector<double>().swap(kc);
            MSD_cutoff=cast_d(dat.size())/10.0;
            make_xi();
            for (size_t i=0; i<mcIterations; ++i){ 
                make_new_coordinates();
                for (size_t j=0; j<MSD_cutoff; ++j){
                    make_mc(j);
                }
                kc.push_back(make_kc());
                std::vector<double>().swap(mc);
            }
            kc_final_value=get_kc_median();
            std::vector<double>().swap(kc);
            return ;
        }
        double get_value(){
            return kc_final_value;
        }
        void print_pcqc(std::vector<double>& input){
            std::vector<double>().swap(dat);
            dat=input;
            make_sum();
            make_new_coordinates();
            for (size_t i=0; i<pc_vector.size(); ++i){
                std::cout<<pc_vector[i]<<'\t'<<qc_vector[i]<<std::endl;
            }
            return ;
        }
        void print_pcqc(){
            make_sum();
            make_new_coordinates();
            for (size_t i=0; i<pc_vector.size(); ++i){
                std::cout<<pc_vector[i]<<'\t'<<qc_vector[i]<<std::endl;
            }
            return ;
        }
        void clear(){
            std::vector <double>().swap(dat);
            std::vector <double>().swap( dat_sum);
            std::vector <double>().swap( qc_vector);
            std::vector <double>().swap( pc_vector);
            std::vector <double>().swap( mc);
            std::vector <double>().swap( kc);
            std::vector <double>().swap( xi);
            deltaT=1.0;
            MSD_cutoff=0.0;
        }
    private:
        double cast_d(unsigned int input){
            return static_cast<double>(input);
        }
        double get_random01() {
            static std::mt19937 gen(std::random_device{}());
            static std::uniform_real_distribution<double> dist(0.0, 1.0);
            return dist(gen);
        }
        void make_sum(){
            double temp=0.0;
            std::vector<double>().swap(dat_sum);
            for (size_t i=0; i<dat.size(); ++i){
                temp+=dat[i];
                dat_sum.push_back(temp);
            }
            return;
        }
        void make_new_coordinates(){
            std::vector<double>().swap(pc_vector);
            std::vector<double>().swap(qc_vector);
            double temp_pc=0.0;
            double temp_qc=0.0;
            double c_value= get_random01()*my_pi*0.98+0.01;
            double theta;
            for (size_t i=0; i< dat.size(); ++i){
                if (noise){
                    theta= cast_d(i+1)*c_value; 
                }
                else{
                    theta= cast_d(i+1)*c_value + dat_sum[i];
                }
                temp_pc+=dat[i]*cos(theta);
                temp_qc+=dat[i]*sin(theta);
                pc_vector.push_back(temp_pc);
                qc_vector.push_back(temp_qc);
            }
            return;
        }
        void make_mc(unsigned int tope){
            double temp=0.0;
            double temp_qc;
            double temp_pc;
            for (size_t i=0; i< (dat.size()-tope); ++i){
                temp_pc= pc_vector[i]-pc_vector[i+tope];
                temp_qc= qc_vector[i]-qc_vector[i+tope];
                temp+=(temp_pc*temp_pc + temp_qc*temp_qc );
            }
            temp/=cast_d(dat.size()-tope);
            mc.push_back(temp*deltaT*deltaT);
            return;
        }
        void make_xi(){
            std::vector <double>().swap(xi);
            for (size_t i=0; i<MSD_cutoff ; ++i){
                xi.push_back( cast_d( i+1));
            }
            return ;
        }
        double mean_vector(const std::vector<double>& input) {
            if (input.empty()) {
                throw std::invalid_argument("Vector vacío");
            }
            double sum = 0.0;
            for (double x : input) {
                sum += x;
            }
            return sum / cast_d(input.size());
        }
        double cov_vector(const std::vector<double>& left, double left_mean, const std::vector<double>& right, double right_mean) {
            if (left.size() != right.size() || left.empty()) {
                throw std::invalid_argument("Vectores inválidos");
            }
            double cov = 0.0;
            for (size_t i = 0; i < left.size(); ++i) {
                cov += (left[i] - left_mean) * (right[i] - right_mean);
            }
            return cov / cast_d(left.size());
        }
        double var_vector(const std::vector<double>& input, double mean) {
            return cov_vector(input, mean, input, mean);
        }
        double make_kc(){
            double kc_val;
            double mc_mean= mean_vector(mc);
            double xi_mean= mean_vector(xi);
            kc_val=cov_vector(mc, mc_mean, xi, xi_mean);
            kc_val/=sqrt( var_vector(mc,mc_mean)*var_vector(xi,xi_mean));
            return kc_val;
        }
        double get_kc_median() {
            std::sort(kc.begin(), kc.end());
            if (mcIterations % 2 == 1) {
                return kc[mcIterations / 2];
            } else {
                return (kc[mcIterations / 2 - 1] + kc[mcIterations / 2]) / 2.0;
            }
        }

};
#endif