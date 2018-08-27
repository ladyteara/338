#include <stdio.h>
#include <mpi.h>

void get_input(double* a, double* b, int* n) {
  printf("Please enter 3 numbers: \n");
  scanf("%lf %lf %d", a, b, n);
}
void main(void) {
  int my_rank, comm_sz;
  double a_p, b_p;
  int n_p;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) 
  {
	get_input(&a_p, &b_p, &n_p);
  }

  MPI_Bcast(&a_p, 1, MPI_DOUBLE, 	0, MPI_COMM_WORLD);
  MPI_Bcast(&b_p, 1, MPI_DOUBLE, 	0, MPI_COMM_WORLD);
  MPI_Bcast(&n_p, 1, MPI_INT, 		0, MPI_COMM_WORLD);
    
  if(my_rank !=0)
  {
    printf("Process %d received %lf, %lf, and %d\n", my_rank, a_p, b_p, n_p); 
  }
  MPI_Finalize();
}

