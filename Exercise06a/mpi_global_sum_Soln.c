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
  const int local_n = 10;  //Number of numbers for each process
  //Declare an int array of size global_n to hold all the numbers
  int global_nums[40];
  //Declare an array of size local_n to hold the local numbers
  int local_nums[local_n];
  int local_sum; //Declare an int to hold the local sum
  int global_sum; //Declare an int to hold the global sum
  //Add MPI boiler plate here
  int comm_sz, my_rank;
  
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank != 0) {
    /* Get numbers from process 0 using MPI_Scatter
       Calculate local sum
       Participate in global sum calculation: call MPI_Reduce
    */
	int local_dummy; //Dummy variable to hold a place in MPI_Reduce()
	local_sum = 0;
	MPI_Scatter(global_nums, local_n, MPI_INT, local_nums, local_n,
	            MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < local_n; i++) {
		local_sum += local_nums[i];
	}
	//Send local sum back to destination process
	MPI_Reduce(&local_sum, &local_dummy, 1, MPI_INT, MPI_SUM, 0,
	           MPI_COMM_WORLD);
  } else { //This is the code for process 0
    /* Read numbers from file into array[global_n]
       Distribute numbers using MPI_Scatter()
       Calculate local sum
       Get global sum by calling MPI_Reduce()
       Print global sum:
         printf("Global sum: %d\n", variable_name);
    */
	char fname[] = "randnums.txt";
	int total = 0;  //Total for checking global sum
	local_sum = 0;
	global_sum = 0;
	
	readfile(fname, global_nums);
	MPI_Scatter(global_nums, local_n, MPI_INT, local_nums, local_n,
	            MPI_INT, 0, MPI_COMM_WORLD);
	for (int i = 0; i < local_n; i++) {
		local_sum += local_nums[i];
    }
	MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
	           MPI_COMM_WORLD);
	//The following loop adds all the number to check the global sum
	for (int i = 0; i < global_n; i++) total += global_nums[i];
	printf("\n");  //Print a blank line
	printf("Global sum: %d\n", global_sum);
	printf("Total (check): %d\n", total);
  } //end else
  MPI_Finalize();  //Clean up
}
