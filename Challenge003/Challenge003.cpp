#include <iostream>
#include <mpi.h>
#include <Windows.h>

/*Write an MPI program where:
each node will sleep for a random amount of time
amount of time should be between 0-4
print out “rank X sleeping” and the amount of time to sleep
on awakening, the node will then synchronise with the others
print “rank X synchronised” after synchronisation*/

int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	std::cout << "rank " << world_rank << " size " << world_size << std::endl;

	// seed sleep
	srand(world_rank);
	int sleep_time = rand() % 5;
	std::cout << "rank " << world_rank << " sleeping " << sleep_time << std::endl;

	// send to sleep
	Sleep(sleep_time * 1000);
	std::cout << "rank " << world_rank << " waking up and synchronizing" << std::endl;

	// wake up and synchronise 
	MPI_Barrier(MPI_COMM_WORLD);
	std::cout << "rank " << world_rank << " synchronized" << std::endl;

	MPI_Finalize();
	return 0;
}