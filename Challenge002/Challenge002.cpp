#include <iostream>
#include <mpi.h>
#include <ctime>

//Write an MPI program where:
// each node generates a random number
// &subsequently sends to the coordinator
// which will then display the total sum

int main(int argc, char** argv) {
	//store current time ms
	time_t now = time(nullptr);

	MPI_Init(NULL, NULL);
	
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	//seed random number
	srand(world_rank + now);

	// random and total variables
	int our_sum = rand() % 100;
	int total_sum = 0;

	// 1 variable to be reduced, 2 destination variable contain reduced value 
	// 3 count, 4 type, 5 reduce operation(ex:min,max), 6 root rank, communicator
	MPI_Reduce(&our_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	// print out
	std::cout << "rank " << world_rank << " our sum, total sum: " << our_sum <<
		", " << total_sum << std::endl;


	MPI_Finalize();
	return 0;
}