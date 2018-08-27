/* 	Tara Walton  tara1984
	Assign 2, Monte Carlo - 4/4/17
	Assignment uses a promt to get a number of tosses from the user. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <math.h>


double monte(unsigned throws) {
	/*returns the number of throws in the circle*/

	unsigned monteResult = 0, n = 0, m = 0;		
	double x = 0, y = 0;
 
	for(int i=0; i<throws; i++)
	{	
		//generate n for negative values
		n = rand() % 2;
		m = rand() % 2;
		//generate x
		x = pow(-1, n)*(rand()/(double)RAND_MAX);
			//printf("X:   %lf\n", x);
		//generate y 
		y = pow(-1, m)*(rand()/(double)RAND_MAX);
			//printf("Y:   %lf\n", y);
  
		//check for hit within unit circle
			//printf("%f\n", x*x + y*y);
		if (x*x + y*y <= 1)
		{	
			monteResult++; 	
		}
	}
  
	printf("Monte Carlo hits %d out of %d throws\n", monteResult, throws);
	return monteResult;
}

void get_input(long long int* toss) {
	/*Get user input*/
	printf("Please enter the number of throws: \n");
	scanf("%d", toss);
}

double piEstimate(int n, int throws){
	/*Calculate pi estimate*/
	return 4*n/(double)throws;
}


int main(void) {
	long long int tosses = 0;
	long long int procToss = 0;
	long long int inCircle = 0;				//global total from all processes	
	long long int monteResult = 0;			//local total to each process
    double pi = 0;
	
	//Add MPI boiler plate here
	int comm_sz; 							//Number of processes
	int my_rank; 							//Rank of local process
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	//printf("%d", my_rank);
    srand(my_rank + time(NULL));
	
	if (my_rank == 0) 						//Primary Process
	{
		//Get number of tosses
		//Broadcast tosses
		//Reduce the sum of landed tosses
		get_input(&tosses);
		procToss = tosses / comm_sz;
			//printf("Number of tosses for procToss process: %d\n", procToss);
	
		MPI_Bcast(&procToss, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
		monteResult = monte(procToss);
			//printf("Rank %d:     In Circle: %d  Total:%d\n", my_rank, monteResult, tosses);	
	
		MPI_Reduce(&monteResult,&inCircle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
		double pi = piEstimate(inCircle, tosses);
		printf("Pi %lf\n", pi);
	}
	
	else									//all other processes
	{
		//printf("R%d\n", my_rank);
		//Monte Carlo
		MPI_Bcast(&procToss, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
		monteResult = monte(procToss);
			//printf("Rank %d:     In Circle: %d  Total:%d\n", my_rank, monteResult, tosses);
	
		int localDummy;
		MPI_Reduce(&monteResult, &localDummy, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	} 
	
	MPI_Finalize();
}
