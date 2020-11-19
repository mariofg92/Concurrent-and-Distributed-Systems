#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

const unsigned long n = 2;		// número de hebras
const int num_items = 50;		// número de elementos a producir/consumir 
const int tam_buffer = 10;		// tamaño del buffer intermedio
static int buffer[tam_buffer];	// buffer intermedio
static int primera_libre = 0;		// índice de la primera posición libre
static int primera_ocupada = 0;	// índice de la primera posición ocupada

sem_t puede_consumir;
sem_t puede_producir;

int producir_dato()
{
	static int contador = 1;
	return contador++;
}

void consumir_dato(int dato)
{
	cout << "dato recibido: " << dato << endl;
}

void * productor(void *) 
{
	for(unsigned long i = 0; i < num_items; i++)
	{
		int dato = producir_dato();

		sem_wait(&puede_producir);
		buffer[primera_libre] = dato;
		sem_post(&puede_consumir);
	
		primera_libre = (primera_libre + 1) % tam_buffer;

	}
	return NULL;
}

void * consumidor(void *)
{
	for(unsigned i = 0; i < num_items; i++)
	{
		int dato;
	
		sem_wait(&puede_consumir);
		dato = buffer[primera_ocupada];
		sem_post(&puede_producir);

		consumir_dato(dato);	
		primera_ocupada = (primera_ocupada + 1) % tam_buffer;		
	}
	return NULL;
}

int main()
{
	sem_init(&puede_consumir,0,0);
	sem_init(&puede_producir,0,tam_buffer);
	
	pthread_t hebra_productora, hebra_consumidora;

	pthread_create(&hebra_productora,NULL,productor,NULL);
	pthread_create(&hebra_consumidora,NULL,consumidor,NULL);

	pthread_join(hebra_productora,NULL);
	pthread_join(hebra_consumidora,NULL);

	sem_destroy(&puede_producir);
	sem_destroy(&puede_consumir);

	cout << "FIN" << endl;
}
