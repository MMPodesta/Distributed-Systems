#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
	// initialise the MPI library
	MPI_Init(NULL, NULL);

	// Determine the world size
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Determine our rank in the world
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// print out the rank and size
	std::cout << "rank " << world_rank << " size " << world_size << std::endl;

	// Lets imagine that we have a local sum in each node that is 5 and we want to reduce that to a 
	// Single value in the root node that is to be displayed to the user
	int our_sum = 5;
	int total_sum = 0;
	
	// 1 variable to be reduced, 2 destination variable contain reduced value 
	// 3 count, 4 type, 5 reduce operation(ex:min,max), 6 root rank, communicator 
	MPI_Reduce(&our_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	std::cout << "rank " << world_rank << " our_sum, total_sum: " << our_sum << ", " << 
		total_sum << std::endl;

	// finalise MPI library
	MPI_Finalize();

	return 0;
}