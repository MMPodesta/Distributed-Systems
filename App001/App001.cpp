#include <iostream>
#include <mpi.h>
int main(int argc, char** argv) {
	// see if we can initialise the mpi library this is always the
	// first thing that we must do at the start of an MPI program
	MPI_Init(NULL, NULL);
	// one of the important tasks that we have to establish is how many processes are
	// in this MPI instance. this will determine who and what we have to communicate with
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	std::cout << "world size is: " << world_size << std::endl;
	// another important job is finding out which rank we are. we can use this rank
	// number to assign seperate jobs to different mpi units
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	std::cout << "world rank is: " << world_rank << std::endl;
	// before we can end our application we need to finalise everything so MPI can shut
	// down properly
	MPI_Finalize();
	// simple hello world just to verify that everything is working for us
	std::cout << "hello world" << std::endl;
	return 0;
}


