#include "mpi.h"
#include <iostream>
#include "math.h"
#include <time.h>
#include <stdlib.h>

#define Productor 0
#define Buffer  1
#define Consumidor 2
#define ITERS 20

using namespace std;


void productor(){ 
for (unsigned int i=0;i<ITERS;i++){ 
  cout<< "Productor produce valor "<<i<<endl<<flush;
  sleep(rand() % 2 );
  MPI_Ssend( &i, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD );
 }
}



void buffer(){
 int value,peticion; 
 MPI_Status status;
 for (unsigned int i=0;i<ITERS;i++){ 
  MPI_Recv(&value,    1, MPI_INT, Productor, 0, MPI_COMM_WORLD,&status );
  MPI_Recv(&peticion, 1, MPI_INT, Consumidor, 0, MPI_COMM_WORLD,&status );
  cout<< "Buffer recibe valor "<< value << " de Productor "<<endl<<flush;  
  MPI_Ssend( &value,   1, MPI_INT, Consumidor, 0, MPI_COMM_WORLD); 
  cout<< "Buffer envía valor "<< value << " a Consumidor "<<endl<<flush;  
 }
}  
  

void consumidor(){
 int value,peticion=1; float raiz;
 MPI_Status status;
 for (unsigned int i=0;i<ITERS;i++){
  MPI_Ssend(&peticion, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD); 
  MPI_Recv(&value, 1,     MPI_INT, Buffer, 0, MPI_COMM_WORLD,&status );
  cout<< "Consumidor recibe valor "<<value<<" de Buffer "<<endl<<flush;
  sleep(rand() % 2 );
  raiz=sqrt(value);
 }
}





int main(int argc, char *argv[]) {
  int rank,size; 
  cout.flush();
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  /* Inicializa la semilla aleatoria */
  srand ( time(NULL) );
  if (size!=3) 
   {cout<< "El numero de procesos debe ser 3 "<<endl;return 0;} 
  if (rank == Productor) productor();
   else if (rank==Buffer) buffer();
    else consumidor();
  MPI_Finalize( );
  return 0;
}

