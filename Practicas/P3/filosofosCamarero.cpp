#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define TAG_TENEDOR 0
#define TAG_SENTARSE 1
#define TAG_LEVANTARSE 2
#define camarero 10

using namespace std;

void Filosofo(int id, int nprocesos);
void Tenedor(int id, int nprocesos);
void Camarero();

int main(int argc,char** argv ){
	int rank,size;
 	srand(time(0));
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	if( size!=11){
		if(rank == 0)
      	cout<<"El numero de procesos debe ser 11" << endl << flush;
   	MPI_Finalize();
		return 0;
 	}

	if(rank == 10)
		Camarero();
 	else{
		if ((rank%2) == 0)
  			Filosofo(rank,size-1); // Los pares son Filosofos 
 		else
   		Tenedor(rank,size-1);  // Los impares son Tenedores
 	}
	MPI_Finalize();
 return 0;
}

void Filosofo(int id, int nprocesos){
	int izq=(id+1)%nprocesos;
	int der=(id-1+nprocesos)%nprocesos;

	int valor=1;
	int puedeSentarse;
	MPI_Status status;
	
	while(1){
		// Pensando
		cout << "Filosofo " << id << " PENSANDO" << endl << flush;
		sleep((rand()%3)+1 );

		// Sentarse en la mesa
		MPI_Ssend(&valor,1,MPI_INT,camarero,TAG_SENTARSE,MPI_COMM_WORLD);
		MPI_Recv(&puedeSentarse,1,MPI_INT,camarero,TAG_SENTARSE,MPI_COMM_WORLD,&status);

		if(puedeSentarse == 1){
			cout << "Filosofo " << id << " se sienta" << endl << flush;

			// Solicita tenedor izquierdo
			MPI_Ssend(&valor,1,MPI_INT,izq,TAG_TENEDOR,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " coge su tenedor izquierdo (" << izq << ")" << endl << flush;

			// Solicita tenedor derecho
			MPI_Ssend(&valor,1,MPI_INT,der,TAG_TENEDOR,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " coge su tenedor derecho (" << der << ")" << endl << flush;

			// Comiendo
			cout << "Filosofo " << id << " COMIENDO" << endl << flush;
			sleep((rand() % 3)+1);

			// Suelta el tenedor 
			MPI_Ssend(&valor,1,MPI_INT,izq,TAG_TENEDOR,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " suelta tenedor izquierdo (" << izq << ")" << endl << flush;

			// Suelta el tenedor derecho
			MPI_Ssend(&valor,1,MPI_INT,der,TAG_TENEDOR,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " suelta tenedor derecho (" << der << ")" << endl << flush;

			// Levantarse de la mesa
			MPI_Ssend(&valor,1,MPI_INT,camarero,TAG_LEVANTARSE,MPI_COMM_WORLD);
			MPI_Recv(&valor,1,MPI_INT,camarero,TAG_LEVANTARSE,MPI_COMM_WORLD,&status);
			cout << "Filosofo " << id << " se levanta" << endl << flush;
		}
 	}
}

void Camarero(){
	int valor;
	MPI_Status status;
	int comensales = 0;
	int puedeSentarse = 1;
	int noPuedeSentarse = 0;

	while(1){
		MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if(status.MPI_TAG == TAG_SENTARSE){
			MPI_Recv(&valor,1,MPI_INT,status.MPI_SOURCE,TAG_SENTARSE,MPI_COMM_WORLD,&status);
			if(comensales < 4){
				MPI_Ssend(&puedeSentarse,1,MPI_INT,status.MPI_SOURCE,TAG_SENTARSE,MPI_COMM_WORLD);	
				comensales++;
			}else{
				MPI_Ssend(&noPuedeSentarse,1,MPI_INT,status.MPI_SOURCE,TAG_SENTARSE,MPI_COMM_WORLD);
				cout << "Filosofo " << status.MPI_SOURCE << " no se puede sentar, MESA LLENA!!!" << endl << flush;
			}
		}else if(status.MPI_TAG == TAG_LEVANTARSE){
			MPI_Recv(&valor,1,MPI_INT,status.MPI_SOURCE,TAG_LEVANTARSE, MPI_COMM_WORLD, &status);
			MPI_Ssend(&valor,1,MPI_INT,status.MPI_SOURCE,TAG_LEVANTARSE,MPI_COMM_WORLD);
			comensales--;
		}
	}
}

void Tenedor(int id, int nprocesos){	
	int valor;
	MPI_Status status;

	while(1){
   	MPI_Recv(&valor,1,MPI_INT,MPI_ANY_SOURCE,TAG_TENEDOR,MPI_COMM_WORLD,&status);
    	//cout << "Tenedor " << id << " recibe peticion de " << status.MPI_SOURCE << endl << flush;

    	MPI_Recv(&valor,1,MPI_INT,status.MPI_SOURCE,TAG_TENEDOR,MPI_COMM_WORLD,&status);
    	//cout << "Tenedor " << id << " recibe liberacion de " << status.MPI_SOURCE << endl << flush;
	}
}
