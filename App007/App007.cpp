#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <Windows.h>

int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);
	
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


	std::cout << "rank " << world_rank <<  " size " << world_size 
		<< std::endl;

	// seed sleep 
	srand(world_rank);
	int sleep_time = rand() % 5;
	std::cout << "node " << world_rank << " sleeping for " << sleep_time << " seconds." <<
		std::endl;

	// sleep
	Sleep(sleep_time * 1000);
	std::cout << "node " << world_rank << " exiting sleep and synchronising" << std::endl;

	// barrier for synchronisation 
	MPI_Barrier(MPI_COMM_WORLD);
	std::cout << "node " << world_rank << " synchronised" << std::endl;


	MPI_Finalize();
	return 0;
}