#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> 

using namespace std;

const unsigned long n = 2;		// número de hebras
const int num_items = 50;		// número de elementos a producir/consumir 
const int tam_buffer = 10;		// tamaño del buffer intermedio
static int buffer1[tam_buffer];	// buffer intermedio
static int buffer2[tam_buffer];	// buffer intermedio
static int primera_libre1 = 0;		// índice de la primera posición libre
static int primera_libre2 = 0;		// índice de la primera posición libre

sem_t puede_consumir1;
sem_t puede_consumir2;
sem_t puede_producir1;
sem_t puede_producir2;
sem_t mutex1;
sem_t mutex2;

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

		if(rand()%2 == 0){
			sem_wait(&puede_producir1);
			sem_wait(&mutex1);
			buffer1[primera_libre1] = dato;
			primera_libre1 = (primera_libre1 + 1) % tam_buffer;
			sem_post(&mutex1);
			sem_post(&puede_consumir1);
		}else{
			sem_wait(&puede_producir2);
			sem_wait(&mutex2);
			buffer2[primera_libre2] = dato;
			primera_libre2 = (primera_libre2 + 1) % tam_buffer;
			sem_post(&mutex2);
			sem_post(&puede_consumir2);
		}
	}
	return NULL;
}

void * consumidor(void *)
{
	for(unsigned i = 0; i < num_items; i++)
	{
		int dato;
	
		if(i%2==0){
			sem_wait(&puede_consumir1);
			sem_wait(&mutex1);
			primera_libre1 =  (primera_libre1 + tam_buffer - 1) % tam_buffer;
			dato = buffer1[primera_libre1];
			sem_post(&mutex1);
			consumir_dato(dato);
			sem_post(&puede_producir1);
		}else{
			sem_wait(&puede_consumir2);
			sem_wait(&mutex2);
			primera_libre2 =  (primera_libre2 + tam_buffer - 1) % tam_buffer;
			dato = buffer2[primera_libre2];
			sem_post(&mutex2);
			consumir_dato(dato);
			sem_post(&puede_producir2);
		}
	}
	return NULL;
}

int main()
{
	sem_init(&puede_consumir1,0,0);
	sem_init(&puede_producir1,0,tam_buffer);
	sem_init(&mutex1,0,1);
	sem_init(&puede_consumir2,0,0);
	sem_init(&puede_producir2,0,tam_buffer);
	sem_init(&mutex2,0,1);
	
	pthread_t hebra_productora, hebra_consumidora;

	pthread_create(&hebra_productora,NULL,productor,NULL);
	pthread_create(&hebra_consumidora,NULL,consumidor,NULL);

	pthread_join(hebra_productora,NULL);
	pthread_join(hebra_consumidora,NULL);

	sem_destroy(&puede_producir1);
	sem_destroy(&puede_consumir1);
	sem_destroy(&mutex1);
	
	sem_destroy(&puede_producir2);
	sem_destroy(&puede_consumir2);
	sem_destroy(&mutex2);
	cout << "FIN" << endl;
}
