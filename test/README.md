# Test
```cpp
#include "01-test.h"
int main() {
    std::vector<double> dat;//datos del mapa

    Test01 test;
    test.print_pcqc(dat);
    test.clear();
    return 0;
}
```
compilado:  ``` g++ -o main main.cpp ```

guardado de datos pc-qc:  ``` ./main > dat.dat ```

## Dinámica en el espacio Pc-Qc de una serie de tiempo caótico

![Alt text](./test_image_caos.png )

## Dinámica en el espacio Pc-Qc de una serie de tiempo no caótico

![Alt text](./test_image_no_caos.png )

## Comparación raw vs raw + noise

```cpp
#include "./../src/01-test.h"
#include <fstream>

//mapa logístico
double get_logistic(double r, double xn){
    xn=xn*r*(1.0-xn);
    return xn;
}

//para inicializar y para agregar ruido
double get_random(double min, double max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

int main(){
    std::ofstream archivo("dat.dat", std::ios::out | std::ios::trunc); //para guardar datos en archivo dat.dat

    std::vector<double> dat_logistic;
    std::vector<double> dat_logistic_noise;

    double r;
    double xn;
    double xn_noise;

    Test01 test;
    for (int i=0; i<1000; i++ ){
        r=get_random(3.5,3.99);
        archivo<<r<<'\t'; // guardado en archivo

        std::cout<<i<<"\t"<<r<<std::endl;

        xn=get_random(0.01,0.99);
        xn_noise=xn + 0.1*get_random(-1,1); //10% noise

        for (int j=0; j<2100; j++ ){
            xn=get_logistic(r,xn);
            xn_noise=xn + 0.1*get_random(-1,1); //10% noise
            if (j>1000){ // se descarta las primeras 1000 observaciones 
                dat_logistic.push_back(xn); // datos sin ruido
                dat_logistic_noise.push_back(xn_noise); //datos con ruido
            }
        }

        test.init(dat_logistic,false);
        test.make_test();
        archivo<<test.get_value()<<'\t'; // guardado en archivo
        test.clear();

        test.init(dat_logistic_noise, true);
        test.make_test();
        archivo<<test.get_value()<<std::endl; // guardado en archivo
        test.clear();

        std::vector <double>().swap(dat_logistic);
        std::vector <double>().swap(dat_logistic_noise);
    }
    archivo.close();
    
    return 0;
}
```


![Alt text](./test_image_raw_noise.png )

$k_c \approx 1$ evidencia de caos

$k_c \approx 0$ evidencia de no caos