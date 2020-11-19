#include "mpi.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void Filosofo(int id, int nprocesos);
void Tenedor (int id, int nprocesos);

int main(int argc,char** argv ){
	int rank,size;
 	srand(time(0));
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	if( size!=10){
		if(rank == 0)
      	cout<<"El numero de procesos debe ser 10" << endl << flush;
   	MPI_Finalize();
		return 0;
 	}

 	if ((rank%2) == 0)
  		Filosofo(rank,size); // Los pares son Filosofos 
 	else
   	Tenedor(rank,size);  // Los impares son Tenedores
 	
	MPI_Finalize( );
 return 0;
}

void Filosofo(int id, int nprocesos){
	int izq=(id+1)%nprocesos;
	int der=(id-1+nprocesos)%nprocesos;

	int valor=1;
	
	while(1){
		// Para eliminar posibilidad de interbloqueo, fuerzo al filosofo 0 a coger primero el tenedor derecho
		if(id == 0){
			// Solicita tenedor derecho
			cout << "Filosofo " << id << " solicita tenedor derecho (" << der << ")" << endl << flush;
			MPI_Ssend(&valor,1,MPI_INT,der,0,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " coge su tenedor derecho (" << der << ")" << endl << flush;

			// Solicita tenedor izquierdo
			cout << "Filosofo " << id << " solicita tenedor izquierdo (" << izq << ")" << endl << flush;
			MPI_Ssend(&valor,1,MPI_INT,izq,0,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " coge su tenedor izquierdo (" << izq << ")" << endl << flush;
		}else{
			// Solicita tenedor izquierdo
			cout << "Filosofo " << id << " solicita tenedor izquierdo (" << izq << ")" << endl << flush;
			MPI_Ssend(&valor,1,MPI_INT,izq,0,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " coge su tenedor izquierdo (" << izq << ")" << endl << flush;

			// Solicita tenedor derecho
			cout << "Filosofo " << id << " solicita tenedor derecho (" << der << ")" << endl << flush;
			MPI_Ssend(&valor,1,MPI_INT,der,0,MPI_COMM_WORLD);
			cout << "Filosofo " << id << " coge su tenedor derecho (" << der << ")" << endl << flush;
		}

		// Comiendo
		cout << "Filosofo " << id << " COMIENDO" << endl << flush;
		sleep((rand() % 3)+1);

		// Suelta el tenedor 
		MPI_Ssend(&valor,1,MPI_INT,izq,0,MPI_COMM_WORLD);
		cout << "Filosofo " << id << " suelta tenedor izquierdo (" << izq << ")" << endl << flush;

		// Suelta el tenedor derecho
		MPI_Ssend(&valor,1,MPI_INT,der,0,MPI_COMM_WORLD);
		cout << "Filosofo " << id << " suelta tenedor derecho (" << der << ")" << endl << flush;

		// Pensando
		cout << "Filosofo " << id << " PENSANDO" << endl << flush;
		sleep((rand()%3)+1 );
 	}
}

void Tenedor(int id, int nprocesos){	
	int value;
	MPI_Status status;

	while(1){
		// Recibe la peticion de un filosofo. No es necesario controlar qué filosofo
		// es el que le envia la petición, ya que los únicos filosofos que envian peticiones 
		// a un tenedor son el de su izquierda y el de su derecha. (id-1 y id+1)
   	MPI_Recv(&value,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
    	cout << "Tenedor " << id << " recibe peticion de " << status.MPI_SOURCE << endl << flush;

    	// Espera a que el filosofo suelte el tenedor, cuando el tenedor está en uso,
		// solo se aceptan peticiones del filosofo que lo está usando
    	MPI_Recv(&value,1,MPI_INT,status.MPI_SOURCE,0,MPI_COMM_WORLD,&status);
    	cout << "Tenedor " << id << " recibe liberacion de " << status.MPI_SOURCE << endl << flush;
	}
/*	
	int value;
	MPI_Status status;
	int Filo;

	while(1){		
	//Espera un peticion desde cualquier filosofo vecino ...
		 MPI_Probe(MPI_ANY_SOURCE,0,MPI_COMM_WORLD, &status);
		 Filo=status.MPI_SOURCE;

		 // Recibe la peticion del filosofo ...
		 MPI_Recv(&value,1,MPI_INT,Filo,0,MPI_COMM_WORLD,&status);
		 cout << "Tenedor " << id << " recibe peticion de " << Filo << endl << flush;

		 // Espera a que el filosofo suelte el tenedor...
		 MPI_Recv(&value,1,MPI_INT,Filo,0,MPI_COMM_WORLD,&status);
		 cout << "Tenedor " << id << " recibe liberacion de " << Filo << endl << flush;
	}
*/
}
