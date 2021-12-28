/*
Author: Valeria Insogna
Date: 30/12/2021

This code implement a simple 3d matrix-matrix addition in parallel using virtual topology.
The aim of the exercise is to study its scalability in term of communication within a single THIN node. 

I used collective operations to communicate among MPI processes. 
Program accepts as input the sizes of the matrixes and then allocates the matrix and initialize them using double precision 
random numbers.

Use for matrices the following sizes when running:

2400 x 100 x 100 ;
1200 x 200 x 100 ;
800 x 300 x 100;
*/
//#include <mpi.h>
#include "/usr/lib/x86_64-linux-gnu/openmpi/include/mpi.h" //extended path emacs is unable to find the correct header, very strange, to be checked
#include <stdio.h>  //printf stuff and other
#include <stdlib.h> //need for memory management
#include <string.h> //need for memcopy
#include <unistd.h> // need for sleep()

#define TRUE 1
#define FALSE 0
#define TYPE double
#define M_TYPE MPI_DOUBLE

void print_3D_matrix(TYPE *M, int *matrix_shape) {
  int dept=matrix_shape[2];
  int col=matrix_shape[1];
  int row=matrix_shape[0];
  for (int k = 0; k < dept; ++k) {
    printf("\n---plane %d---\n", k);
    for (int i = 0; i < row; ++i) {
      for (int j = 0; j < col; ++j) {
        printf("%2.3f ", M[i * col + j + k * row * col]);
      }
      printf("\n");
    }
  }
}

                                                                                                                                                                                                                                                                               
int get_elem_count(int *matrix_shape) {                                                                                                                                                       return matrix_shape[0] * matrix_shape[1] * matrix_shape[2];                                                                                                                               }

void init_matrix(TYPE *M, int elem_count) {
  for (int i = 0; i < elem_count; ++i)
    i[M] = (TYPE) i;
    //M[i]=i;
}

long TYPE get_sum(TYPE* m,int elem_count){
  long  TYPE acc=0;
  for(int i=0;i<elem_count;++i)
    acc+=m[i];
  return acc;
}

int main(int argc, char *argv[]) {
  
  MPI_Init(&argc, &argv);
  int rank, size;
  
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  if (argc < 4) {
    if (rank == 0)
      printf("No enought args\n");
    MPI_Finalize();
    return 1;
  }


  // save matrix shape into an array
  int matrix_shape[3];
  matrix_shape[0] = atoi(argv[1]);
  matrix_shape[1] = atoi(argv[2]);
  matrix_shape[2] = atoi(argv[3]);

  // total matrix elements
  int elem_count = get_elem_count(matrix_shape);

  //A B matrix pointer
  TYPE *A = NULL;
  TYPE *B = NULL;
  long TYPE sum_control=0;

  if (rank == 0) {
#ifdef PRINT1
    printf("matrix size = %d X %d X %d\n",matrix_shape[0],matrix_shape[1],matrix_shape[2]);
#endif
    
    //allocate A,B
    A = malloc(sizeof(TYPE) * elem_count);
    B = malloc(sizeof(TYPE) * elem_count);
    
    init_matrix(A, elem_count);
    init_matrix(B, elem_count);
    
    sum_control=get_sum(A,elem_count);
    sum_control+=get_sum(B, elem_count);
    
#ifdef PRINT2
    printf("\nA MATRIX %d\n",elem_count);
    print_3D_matrix(A,matrix_shape);
#endif
            
  }
  
  //elem count of each procs
  int my_elem_count;
  //get integer division
  my_elem_count=elem_count/size;
  //get reminder if needed
  my_elem_count=(rank>=(elem_count % size)) ? my_elem_count : my_elem_count+1;

  //init displ and send_count, necessary only for master proc
  int* displacement=NULL;
  int* send_count=NULL;

  //only master process execute this chunk
  if(rank==0){
    
    displacement=malloc(sizeof(int)*size);
    send_count=malloc(sizeof(int)*size);

    int acc=0;
    for(int i=0; i<size; ++i) {
      send_count[i] = ( i >= (elem_count % size) ) ? elem_count/size : elem_count/size +1;
      displacement[i]=acc;
      acc+=send_count[i];
    }
    
  }
  
#ifdef PRINT1
  printf("I'm %d and I get %d/%d\n",rank,my_elem_count,elem_count);
#endif
  
  //allocate local matrices
  TYPE *sA = malloc(sizeof(TYPE) * my_elem_count);
  TYPE *sB = malloc(sizeof(TYPE) * my_elem_count);
    
  double time=0;
  

    if(rank==0) {
      time=MPI_Wtime();

      MPI_Scatterv(A, send_count, displacement, M_TYPE, sA, my_elem_count, M_TYPE, 0, MPI_COMM_WORLD);
      MPI_Scatterv(B, send_count, displacement, M_TYPE, sB, my_elem_count, M_TYPE, 0, MPI_COMM_WORLD);

      time=MPI_Wtime()-time;
      printf("Scatter overhead master $%f\n",time);
    }else {
      
      MPI_Scatterv(NULL, NULL, NULL, M_TYPE, sA, my_elem_count, M_TYPE, 0, MPI_COMM_WORLD);
      MPI_Scatterv(NULL, NULL, NULL, M_TYPE, sB, my_elem_count, M_TYPE, 0, MPI_COMM_WORLD);
    
    }
    
    time=MPI_Wtime();

    for(int i=0; i<my_elem_count; ++i)
      sA[i]+=sB[i];

    time=MPI_Wtime()-time;
    printf("parallel time $%f\n",time);

    free(sB);
    
    
    TYPE* C=NULL;
    
    if(rank==0)
      C=malloc(elem_count*sizeof(TYPE));

    time=MPI_Wtime();

    MPI_Gatherv(sA, my_elem_count, M_TYPE, C, send_count, displacement, M_TYPE, 0, MPI_COMM_WORLD);

    time=MPI_Wtime()-time;
    if(rank==0)
      printf("gather time %f \n",time);

    //free sA, no longer needed
    free(sA);

    if(rank==0){
      free(A);
      free(B);
      
#ifdef PRINT2
      printf("\nC MATRIX %d\n",elem_count);
      print_3D_matrix(C,matrix_shape);
#endif
      if(get_sum(C, elem_count)==sum_control)
	printf("Risultato corretto\n");
      else
	printf("Errore!");
      free(C);
    }
    
    MPI_Finalize();
}