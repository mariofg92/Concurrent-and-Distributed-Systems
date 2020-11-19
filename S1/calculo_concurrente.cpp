
#include <iostream>
#include <pthread.h>
#include "fun_tiempo.h"

using namespace std ;

const unsigned long m = 200000;	// número de muestras
const unsigned long n = 4; // número de hebras
double resultado_parcial[n]; // vector de resultados parciales

double f(double x)	// implementa función f	
{	
	return 4.0/(1+x*x);	
}

void * funcion_hebra(void *ih_void) // función que ejecuta cada hebra
{
	unsigned long ih = (unsigned long) ih_void; // número o índice de esta hebra
	double sumap = 0.0;
	
	// calcular suma parcial en "sumap"
	for(unsigned long i = ih*(m/n); i < ih*(m/n)+(m/n); i++)
		sumap += f((i+0.5)/m);

	resultado_parcial[ih] = sumap; // guardar suma parcial en vector.
}

double calcular_integral_concurrente()
{
	pthread_t id_hebra[n];
	
	for(unsigned i = 0; i < n; i++)
		pthread_create(&(id_hebra[i]), NULL, funcion_hebra, (void *)i);
	
	for(unsigned i = 0; i < n; i++)
		pthread_join(id_hebra[i],NULL);

	double suma = 0.0;
	
	for(unsigned i = 0; i < n; i++)	
		suma += resultado_parcial[i];

	return suma/m;		 
}

int main()
{
	struct timespec inicio = ahora();

	cout << calcular_integral_concurrente() << endl;

	struct timespec fin = ahora();

	cout << "Tiempo transcurrido: " << duracion(&inicio,&fin) << " seg." << endl;	
}
