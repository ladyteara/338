/* This version determines how many items to send to
   each process based on the number of processes and
   the number of integers in the input file. The
   number of integers in the file must be evenly
   divisible by the number of processes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int findlen(char* fname) {
  //Returns the number of integers in the file fname
  FILE* f;
  int x;  						//Variable to receive numbers read from the file
  int n = 0;  					//Variable to count numbers in the file
  
  f = fopen(fname, "r");
  while (fscanf(f, "%d", &x) > 0) n++;
  fclose(f);
  printf("Findlen returns %d\n", n);
  return n;
}

int readfile(char* fname, int* nums) {
  //Returns the integers from file fname in nums array
  FILE* f;
  int i;  						//Loop index
  f = fopen(fname, "r");
  i = 0;
  while (fscanf(f, "%d", &nums[i]) > 0) {
    i++;
  }
  fclose(f);
  return i;
}
int main(void) {
  int global_n;  				//n in input file
  int local_n;  				//n for each process
  int* global_nums;				//Holds all numbers
  int* local_nums; 				//Holds local nums
  int local_sum;
  int global_sum;
  //Add MPI boiler plate here
  int comm_sz; 					//Number of processes
  int my_rank; 					//Rank of local process
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if (my_rank != 0) {
    /* Get numbers from process 0 using MPI_Scatter
       Calculate local sum
       Participate in global sum calculation: call MPI_Reduce
    */
    int i; //Loop index
    local_sum = 0;  //Initialize to 0
    //Get local_n through broadcast from process 0
    MPI_Bcast(&local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
    //Next statement prints local_n for debugging
    printf("process %d: %d\n", my_rank, local_n);
	
    //Allocate memory for local_nums DONE
    local_nums = malloc(local_n * sizeof(int));
	
    MPI_Scatter(global_nums, local_n, MPI_INT,
                local_nums, local_n, MPI_INT,
                0, MPI_COMM_WORLD);
    for (i = 0; i < local_n; i++) {
      local_sum += local_nums[i];
    }
    MPI_Reduce(&local_sum, &i, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

  } else {
    /* Read numbers into array[global_n]
       Distribute numbers using MPI_Scatter()
       Calculate local sum
       Get global sum by calling MPI_Reduce()
       Print global sum:
         printf("Global sum: %d\n", var_name);
    */
    char fname[] = "randnums.txt";
    int i;  //Loop index
    local_sum = 0;  //Initialize
    global_sum = 0;
    
    //Get total number of integers in the file (use findlen) DONE
	global_n = findlen(fname);
	
	//Calculate number of ints handled by each process DONE
	local_n = global_n/comm_sz;
	
    //Now broadcast local_n to all processes DONE
    MPI_Bcast(&local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
    //Allocate memory for all nums DONE
	global_nums= malloc(global_n * sizeof(int));
	
    //Allocate memory for local nums DONE
	local_nums = malloc(local_n * sizeof(int));
	
    
    readfile(fname, global_nums);  //Read nums from file into global_nums array
    //Now send each process its chunk of numbers
    MPI_Scatter(global_nums, local_n, MPI_INT,
                local_nums, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    //Calculate local sum
    for (i = 0; i < local_n; i++) {
      local_sum += local_nums[i];
    }
    //Get global sum by combining all local sums
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("Global sum: %d\n", global_sum);
  } //end else
  MPI_Finalize();
}
