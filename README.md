# 01-Test for Chaos Detection in Time Series (Noisy or Noise-Free)
Implementación del 01-test para detectar caos en series de tiempo [1,2,3].

~~Esta implementación solo debe de usarse para datos sin ruido.~~

Update - se añade soporte para datos con ruido y se mejora los números aleatorios usados para la prueba de Monte Carlo.

Para calcular el resultado final del test, el algoritmo usa el método de  coeficiente de correlación [3].

$K_c \approx 1$ - indicios de caos - signs of chaotic behavior [1,2,3]

$K_c \approx 0$ - no hay caos - absence of chaos [1,2,3]

# Uso general
```cpp
#include "01-test.h"
int main() { 

    //datos bajo estudio
    //debe de tener al menos 1000 observaciones [2]
    std::vector<double> dat;
    double kc;   
    
    //se crea el objeto test01
    Test01 test;

    //tiempo de muestreo
    //si lo usas para sistemas dinámicos de tiempo continuo
    //debes de elegir bien dt, dt muy grande o pequeño
    //crea errores en el resultado del test [3]

    //úsalo si no sabes el tiempo de muestreo 
    //y si tus datos NO tienen ruido
    test.init(dat,false);
    //para datos con ruido cambia 'false' a 'true'

    //úsalo si sabes el tiempo de muestreo
    //test.init(dat,dt,false);

    //inicia el test
    test.make_test();

    //obtén el resultado
    kc=test.get_value();

    //en caso de que quieras imprimir solo las coordenadas pc qc
    //úsalo después del init
    //test.print_pcqc();

    //o si prefieres solo pasar datos al argumento
    //test.print_pcqc(dat,false);

    //limpiar objeto test
    test.clear();

    return 0;
}
```


## Uso básico teniendo datos sin ruido generados por un mapa discreto

```cpp
#include "01test.h"
int main(){
    std::vector<double> dat; //donde se guardan los datos
    //Aquí generas tus datos de al menos 1000 observaciones

    //Se declara el objeto Test01
    Test01 test;

    //inicia el test
    test.init(dat, false);
    test.make_test();
    std::cout<<"Kc obtenido: "<< test.get_value() <<std::endl;
    test.clear();
    //termina el test

    return 0;
}
```

# Referencia

[1] Gottwald, G. A., & Melbourne, I. (2004). A new test for chaos in deterministic systems. Proceedings of the Royal Society of London. Series A: Mathematical, Physical and Engineering Sciences, 460(2042), 603-611.

[2] Gottwald, G. A., & Melbourne, I. (2005). Testing for chaos in deterministic systems with noise. Physica D: Nonlinear Phenomena, 212(1-2), 100-110.

[3] Melosik, M., & Marszalek, W. (2016). On the 0/1 test for chaos in continuous systems. Bulletin of the Polish Academy of Sciences. Technical Sciences, 64(3).