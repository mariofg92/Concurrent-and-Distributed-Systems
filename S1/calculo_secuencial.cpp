
#include <iostream>
#include "fun_tiempo.h"

using namespace std ;

unsigned long m = 200000;	// número de muestras

double f(double x)	// implementa función f	
{	
	return 4.0/(1+x*x);	
}

double calcular_integral_secuencial()
{
	double suma = 0.0;
	for(unsigned long i = 0; i < m; i++)
		suma += f((i+0.5)/m);
	
	return suma/m;
}

int main()
{
	struct timespec inicio = ahora();

	cout << calcular_integral_secuencial() << endl;

	struct timespec fin = ahora();

	cout << "Tiempo transcurrido: " << duracion(&inicio,&fin) << " seg." << endl;	
}
