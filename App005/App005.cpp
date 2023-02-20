/** simple program to show how the MPI broadcast function works **/
/** includes **/
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
	// initialise the MPI library
	MPI_Init(NULL, NULL);

	// determine the world size
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// determine our rank in the world
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// print out the rank and size
	std::cout << "rank " << world_rank << " size " << world_size << std::endl;

	if (world_rank == 0) {
		// broadcast a message to all the other nodes
		int message = 0xDEADBEEF;

		// buffer to send from, count, type, root of communication, communicator 
		MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
		std::cout << "rank 0 broadcasting " << message << std::endl;
	}
	else {
		// recieve a message from the root
		int message = 0;

		// buffer recieve into, count, type, root of communication, communicator 
		MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
		std::cout << "rank " << world_rank << " recieved broadcast of " << message << std::endl;
	}

	// finalise the MPI library
	MPI_Finalize();
}