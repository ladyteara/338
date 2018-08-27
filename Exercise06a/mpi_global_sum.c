/*Tara Walton
Compilation in cygwin: 	mpicc -g -Wall -o name name.c
Run:					mpiexec -n 4 ./name
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int readfile(char fname[], int nums[]) {
  FILE* f;
  int i;  //Loop index
  f = fopen(fname, "r");
  i = 0;
  while (fscanf(f, "%d", &nums[i]) > 0) {
    i++;
  }
  fclose(f);
  return i;
}
int main(void) {
  const int global_n = 40;  //Number of numbers in input file
  const int local_n = 10;  	//Number of numbers for each process
  int glob_nums[global_n];	//Declare an int array of size global_n to hold all the numbers
  int loc_nums[local_n];	//Declare an array of size local_n to hold the local numbers
  int loc_sum = 0;			//Declare an int to hold the local sum
  int glob_sum = 0;			//Declare an int to hold the global sum
  int my_rank, comm_sz;
  
  MPI_Init(NULL, NULL);		//Add MPI boiler plate here
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  if (my_rank != 0) 
  {
	int loc_dummy =0;
    /* Get numbers from process 0 using MPI_Scatter
       Calculate local sum
       Participate in global sum calculation: call MPI_Reduce
    */
	MPI_Scatter(glob_nums, local_n, MPI_INT, loc_nums, local_n, MPI_INT, 0, MPI_COMM_WORLD);
	for(int i=0; i<local_n; i++)  //?
	{
		loc_sum +=loc_nums[i];
	}
	MPI_Reduce(&loc_sum, &loc_dummy, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
  } 
  else 
  {
    /* Read numbers from file into array[global_n]
       Distribute numbers using MPI_Scatter()
       Calculate local sum
       Get global sum by calling MPI_Reduce()
       Print global sum:
         printf("Global sum: %d\n", variable_name);
    */
	char fname[] = "randnums.txt";
	readfile(fname, glob_nums);		//read function
	//int loc_dummy = 0;
	
	MPI_Scatter(glob_nums, local_n, MPI_INT, loc_nums, local_n, MPI_INT, 0, MPI_COMM_WORLD);	//send to other processes
	for(int i = 0; i <global_n; i++)	//?
	{
		loc_sum += loc_nums[i];
	}
	MPI_Reduce(&loc_sum, &glob_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	//Check Step
	int total =0; 						
	for (int j = 0; j < global_n; j++)
	{
		total += glob_nums[j];
	}
	
	printf("Global sum      : %d\n", glob_sum);
	printf("Total validation: %d\n", total);
	

  } //end else
  MPI_Finalize();  //Clean up
}
