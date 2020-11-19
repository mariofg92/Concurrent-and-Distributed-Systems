#include <iostream>
#include <time.h>
#include <unistd.h> 
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

using namespace std;

sem_t puede_fumar1;
sem_t puede_fumar2;
sem_t puede_fumar3;
sem_t puede_reponer;
sem_t mutex;

void fumar(unsigned long fumador)
{ 
	const unsigned miliseg = 100U + (rand() % 1900U) ;
	usleep( 1000U*miliseg );

	sem_wait(&mutex);
	cout<<"Acaba de fumar el fumador " << fumador << endl;
	sem_post(&mutex);
}

int ponerEnMostrador()
{
	return 1 + (rand() % 3);
}

void * estanquero(void *)
{
	while(1){
		sem_wait(&puede_reponer);
		
		int ingrediente = ponerEnMostrador();
		
		cout << "Avisando al fumador " << ingrediente << endl;	

		if(ingrediente == 1)
			sem_post(&puede_fumar1);
		else if(ingrediente == 2)
			sem_post(&puede_fumar2);
		else if(ingrediente == 3)
			sem_post(&puede_fumar3);

		sem_wait(&mutex);
		cout<<"Ha cogido ingrediente el fumador " << ingrediente << endl;
		sem_post(&mutex);	
	}	
}

void * fumador(void *f)
{
	unsigned long fumador = (unsigned long)f;
	
	while(1){
		if(fumador == 1)
			sem_wait(&puede_fumar1);
		else if(fumador == 2)
			sem_wait(&puede_fumar2);
		else if(fumador == 3)
			sem_wait(&puede_fumar3);

		sem_wait(&mutex);
		cout<<"Empieza a fumar el fumador " << fumador << endl;
		sem_post(&mutex);	
		
		sem_post(&puede_reponer);
		
		fumar(fumador);			
	}
}

int main()
{
	srand( time(NULL) ); 
	
	sem_init(&puede_fumar1,0,0);
	sem_init(&puede_fumar2,0,0);
	sem_init(&puede_fumar3,0,0);
	sem_init(&puede_reponer,0,1);
	sem_init(&mutex,0,1);

	pthread_t hebra_fumador1, hebra_fumador2, hebra_fumador3, hebra_estanquero;

	pthread_create(&hebra_fumador1,NULL,fumador,(void*) 1);
	pthread_create(&hebra_fumador2,NULL,fumador,(void*) 2);
	pthread_create(&hebra_fumador3,NULL,fumador,(void*) 3);
	pthread_create(&hebra_estanquero,NULL,estanquero,NULL);

	pthread_join(hebra_fumador1,NULL);
	pthread_join(hebra_fumador2,NULL);
	pthread_join(hebra_fumador3,NULL);
	pthread_join(hebra_estanquero,NULL);
	
	sem_destroy(&puede_fumar1);
	sem_destroy(&puede_fumar2);
	sem_destroy(&puede_fumar3);
	sem_destroy(&puede_reponer);
	sem_destroy(&mutex);

	cout << "FIN" << endl;
}
