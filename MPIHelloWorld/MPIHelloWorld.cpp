#include <iostream>
#include <mpi.h>

int world_size, world_rank;
int main(int argc, char** argv) {
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	std::cout << "Hello commit " << world_rank << ", World: " << world_size << std::endl;
	// finalise MPI and return control to the OS;
	MPI_Finalize();
	return 0;
}


//C:\Users\mauri\source\repos\MPIHelloWorld\x64\Debug\
//mpiexec -n 4 ./MPIHelloWorld.exe
