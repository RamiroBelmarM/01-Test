# 01 Test for Chaos detention in time series or deterministic systems
Implementación del 01-test para detectar caos en series de tiempo.

Esta implementación solo debe de usarse para datos sin ruido.

update - se añade soporte para datos con ruido, ver 01-test.h linea 115

Para calcular el resultado del test, el algoritmo usa el metodo de 
coeficiente de correlacion 

k=1 - indicios de caos

k=0 - no hay caos

```cpp
#include "01-test.h"
int main() {
    //se crea el objeto test01
    Test01 test; 

    //datos bajo estudio
    //debe de tener al menos 1000 observaciones
    std::vector<double> dat;     

    //tiempo de muestreo
    //si lo usas para sistemas dinamicos de tiempo continuo
    //debes de elegir bien dt, dt muy grande o pequeño
    //crea errores en el resultado del test
    //ver referencia
    //double dt=0.1; 

    //usalo si no sabes el tiempo de muestreo
    test.init(dat);

    //usalo si sabes el tiempo de muestreo
    //test.init(dat,dt);

    //inicia el test
    test.make_test();

    //obten el resultado
    test.get_value();

    //en caso de que quieras imprimir solo las coordenadas pc qc
    //test.print_pcqc();

    //pasando datos al argumento
    //test.print_pcqc(dat);

    //limpiar objeto test
    //test.clear();

    return 0;
}
```

# Referencia

Gottwald, G. A., & Melbourne, I. (2004). A new test for chaos in deterministic systems. Proceedings of the Royal Society of London. Series A: Mathematical, Physical and Engineering Sciences, 460(2042), 603-611.

Gottwald, G. A., & Melbourne, I. (2005). Testing for chaos in deterministic systems with noise. Physica D: Nonlinear Phenomena, 212(1-2), 100-110.

Melosik, M., & Marszalek, W. (2016). On the 0/1 test for chaos in continuous systems. Bulletin of the Polish Academy of Sciences. Technical Sciences, 64(3).