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
  int dest;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank == 0) {
    get_input(&a_p, &b_p, &n_p);
    for (dest = 1; dest < comm_sz; dest++) {
      MPI_Send(&a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
      MPI_Send(&n_p, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d received %lf, %lf, and %d\n", my_rank, a_p, b_p, n_p); 
  }
  MPI_Finalize();
}

