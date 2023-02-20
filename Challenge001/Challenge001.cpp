#include <iostream>
#include <mpi.h>
//Write an MPI program that will broadcast an entire array to a set of nodes
// The array should contain 30 random ints
// Make sure to deallocate memory

int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);

	// world size
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	// world rank
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// print their rank and total size
	std::cout << "rank " << world_rank << " size " << world_size << std::endl;

	if (world_rank == 0) {
		// generate array with 30 random ints to send
		int randArray[30];
		for (int i = 0; i < 30; i++){
			randArray[i] = rand() % 100;
		}

		// buffer to send from, count, type, root of communication, communicator 
		MPI_Bcast(&randArray, 30, MPI_INT, 0, MPI_COMM_WORLD);
		
		// check message and rank output
		std::cout << "rank " << world_rank << " broadcasting message: " << std::endl;

		// print sent message array
		for (int i = 0; i < 30; i++) {
			std::cout << randArray[i] << ' ';
		}
		std::cout << '\n' << std::endl;


	}
	else {
		// allocate momery to receive array
		int randArray[30] = {1};

		// buffer to receive from, count, type, root of communication, communicator
		MPI_Bcast(&randArray, 30, MPI_INT, 0, MPI_COMM_WORLD);

		// check message and rank output
		std::cout << "rank " << world_rank << " Received message: " << std::endl;

		// print received message array
		for (int i = 0; i < 30; i++) {
			std::cout << randArray[i] << ' ';
		}
		std::cout << '\n' << std::endl;
	}
	


	// end
	MPI_Finalize();
	return 0;

}