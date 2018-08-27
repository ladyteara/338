#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


double monte(unsigned throws) {
  /*returns the number of throws in the circle*/
  unsigned n = 0;		
  time_t t;
  double x = 0, y = 0;
  
  srand((unsigned) time(&t));
  for(int i=0; i <throws; i++)
  {
	//generate x
	x = rand()/double(RAND_MAX);
	printf("%d", x);
	//generate y
	y = rand()/double(RAND_MAX);
	printf("%d", y);
  
	//check for hit within unit circle
	if (x*x + y*y <= 1)
	{	n+=1; 	}
  }
  
  printf("Monte Carlo hits %d out of %d", n, throws);
  return n;
}

void get_input(double* toss) {
  /*Get user input*/
  printf("Please enter the number of throws: \n");
  scanf("%d", toss);
}

double piEstimate(int n, int throws){
	/*Calculate pi estimate*/
	pi_estimate = 4*n/(double(throws));
	return pi_estimate;
}


int main(void) {
  long long int tosses = 0;
  long long int inCircle = 0;				//global total from all processes	
  long long int monteResult = 0;			//local total to each process
  
  
  
  //Add MPI boiler plate here
  int comm_sz; 					//Number of processes
  int my_rank; 					//Rank of local process
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    
  if (my_rank == 0) 			//Primary Process
  {
	//Get number of tosses
	//Broadcast tosses
	//Reduce the sum of landed tosses
	get_input(&tosses);
	long long int each = tosses / comm_sz;
	printf("Number of tosses for each process: %d", each);
	
	MPI_Bcast(&each, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	unsigned monteResult = 0;		
	time_t t;
	double x = 0, y = 0;
  
	srand((unsigned) time(&t));
	for(int i=0; i <throws; i++)
  {
	//generate x
	x = rand()/double(RAND_MAX);
	printf("%d", x);
	//generate y
	y = rand()/double(RAND_MAX);
	printf("%d", y);
  
	//check for hit within unit circle
	if (x*x + y*y <= 1)
	{	n+=1; 	}
  }
	
	//int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)
	MPI_Reduce(&inCircle, &monteResult, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  }
	
  else							//all other processes
  {
	//Monte Carlo
  unsigned monteResult = 0;		
  time_t t;
  double x = 0, y = 0;
  
  srand((unsigned) time(&t));
  for(int i=0; i <throws; i++)
  {
	//generate x
	x = rand()/double(RAND_MAX);
	printf("%d", x);
	//generate y
	y = rand()/double(RAND_MAX);
	printf("%d", y);
  
	//check for hit within unit circle
	if (x*x + y*y <= 1)
	{	n+=1; 	}
  }
	MPI_Reduce(&inCircle, &monteResult, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
  } 
  
  MPI_Finalize();
}
