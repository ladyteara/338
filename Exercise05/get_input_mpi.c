#include<stdio.h>
#include<mpi.h>

void get_input(double* a, double* b, int* n)
{
	printf("Please enter 3 numbers: ");
	scanf("%lf %lf %d", a, b, n);		//lf (long float)  //d (digit)
	
}


void main(void)
{
	int my_rank, comm_sz;
	double a, b;
	int n, dest;
	
	MPI_Init(NULL, NULL);
	
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	if (my_rank == 0)
	{
		get_input(&a, &b, &n);
		for (dest = 1; dest <comm_sz; dest++)
		{
			//MPI_Send(msg_buf, msg size, msg type, dest, tag, communicator)
			MPI_Send(&a, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(&b, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(&n, 1, MPI_INT, 	dest, 0, MPI_COMM_WORLD);		//don't forget to change data type!
		}
	}
	else
	{
		//MPI_Recv(ms_buff, msg size, msg type, source, tag, communicator, status)
		MPI_Recv(&a, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&b, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&n, 1, MPI_INT, 	0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		printf("Process %d rec'd: %lf, %lf, and %d \n", my_rank, a, b, n);
	}
	
	MPI_Finalize();
}
