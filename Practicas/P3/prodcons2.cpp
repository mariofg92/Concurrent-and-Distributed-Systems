#include "mpi.h"
#include <iostream>
#include "math.h"
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define ITERS 20
#define TAM 5

#define Buffer 5
#define TAG_PRODUCTORES 0
#define TAG_CONSUMIDORES 1

using namespace std;

void imprimeBuffer( int * buffer, int pos){
  cout << " Buffer: ";

	for(int i=0; i<TAM; i++){
	  //Cuando hay datos, como pos marca la última celda con ellos se imprimen hasta ella.
	  if(i<=pos-1)
		 cout << " [ " << buffer[i] << " ] ";
	  else //Se imprime el resto de celdas vacías:
		 cout << " [ ] ";
	}

	cout << endl;
}

void productor(int rank){
  	int value;
	for (unsigned int i=0;i<ITERS;i++){
		value=i;
		cout << "Productor " << rank << " produce " << value << endl << flush;
		sleep(rand() % 2);
		MPI_Ssend(&value, 1, MPI_INT,Buffer,TAG_PRODUCTORES, MPI_COMM_WORLD);
	}
	cout << "Productor " << rank << " ha acabado" << endl << flush;
}

void buffer(){
	int value[TAM]; 
	int pos=0; 
	int peticion;
	int rama;
	MPI_Status status; 

	for (unsigned int i=0;i<ITERS*9;i++){
		if (pos==0){
			rama=0;
		}else if (pos==TAM){
			rama=1;
		}else{
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		  	if (status.MPI_SOURCE <= 4)
				rama=0; 
			else 
				rama=1; 
		}

		switch(rama){
		case 0:
			//Recibimos de cualquier productor:
			MPI_Recv( &value[pos], 1, MPI_INT, MPI_ANY_SOURCE, TAG_PRODUCTORES, MPI_COMM_WORLD,&status);
			cout << "Buffer recibe " << value[pos] << " de productor " << status.MPI_SOURCE << endl << flush;
			pos++;
			//imprimeBuffer(value,pos);
			break;
		case 1:
			//Recibimos una petición de cualquier consumidor
			MPI_Recv( &peticion, 1, MPI_INT, MPI_ANY_SOURCE, TAG_CONSUMIDORES, MPI_COMM_WORLD,&status);
			MPI_Ssend( &value[pos-1], 1, MPI_INT, status.MPI_SOURCE, TAG_CONSUMIDORES, MPI_COMM_WORLD);
			cout << "Buffer envía " << value[pos-1] << " a consumidor " << status.MPI_SOURCE << endl << flush;
			pos--;
			//imprimeBuffer(value,pos);
			break;
		}
	}
}

void consumidor(int rank){
	int value,peticion=1;
	float raiz;
	MPI_Status status;
	for (unsigned int i=0;i<ITERS;i++){
		MPI_Ssend(&peticion, 1, MPI_INT, Buffer, TAG_CONSUMIDORES, MPI_COMM_WORLD);
		MPI_Recv(&value, 1,     MPI_INT, Buffer, TAG_CONSUMIDORES, MPI_COMM_WORLD,&status);
		cout << "Consumidor " << rank << " recibe valor " << value << " de Buffer" << endl << flush;
	 	sleep(rand() % 2);
		raiz=sqrt(value);
	}

	cout << "Consumidor " << rank << " ha acabado" << endl << flush;
}

int main(int argc, char *argv[]) {
	int rank,size;
	cout.flush();

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	srand ( time(NULL) );

	if (size!=10){
		cout << "El numero de procesos debe ser 10" << endl;
		return 0;
	}

	if (rank <= 4)
		productor(rank);
	else if (rank == 5)
		buffer();
	else 
		consumidor(rank);

	MPI_Finalize();
	return 0;
}
