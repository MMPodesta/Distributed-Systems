/** simple program to test the MPI stuff to see if it works **/
/** includes **/
#include <iostream>
#include <cstdlib>
#include <mpi.h>
/** messages for communicating tasks **/
int COMPUTE_AVERAGE = 1;
int SEND_AVERAGE = 2;
int SHUTDOWN = 3;
/** the world rank and size that will be useful in many functions **/
int world_size;
int world_rank;

void master(void) {
	// the total average of all the averages from the nodes
	float total_average = 0;
	// an average that we recieve from a node
	float average = 0;
	// ask all three nodes to compute an average
	for (int i = 1; i < world_size; i++)
		MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	std::cout << "Master (0): told all slaves to compute" << std::endl;
	// ask all three nodes to send their average to us
	for (int i = 1; i < world_size; i++) {
		MPI_Send(&SEND_AVERAGE, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		MPI_Recv(&average, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		total_average += average;
	}
	// take the average of averages and display the result
	std::cout << "Master (0): average result from all slaves is: " << total_average /
		(world_size - 1) << std::endl;
	// tell all the nodes to shutdown
	for (int i = 1; i < world_size; i++) {
		MPI_Send(&SHUTDOWN, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}
	std::cout << "Master (0): shutting down all slaves" << std::endl;
}

void slave(void) {
	// the message type that we have recieved
	int message_type = 0;
	float average = 0;
	// keep looping until we recieve a shutdown message
	MPI_Recv(&message_type, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	while (message_type != SHUTDOWN) {
		if (message_type == COMPUTE_AVERAGE) {
			std::cout << "Slave (" << world_rank << "): calculating average" <<
				std::endl;
			// get the average of 100 random numbers
			srand(world_rank);
			int sum = 0;
			for (int i = 0; i < 100; i++)
				sum += rand() % 10;
			average = sum / 100.f;
			std::cout << "Slave (" << world_rank << "): sum of 100 ints is " << sum
				<< std::endl;
			std::cout << "Slave (" << world_rank << "): average of 100 ints is " <<
				average << std::endl;
			std::cout << "Slave (" << world_rank << "): calculated average" <<
				std::endl;
		}
		else if (message_type == SEND_AVERAGE) {
			MPI_Send(&average, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
			std::cout << "Slave (" << world_rank << "): sent average" << std::endl;
		} MPI_Recv(&message_type, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	std::cout << "Slave (" << world_rank << "): going for shutdown" << std::endl;
}

int main(int argc, char** argv) {
	// see if we can initialise the mpi library this is always the first thing that we
	// must do at the start of an MPI program
	MPI_Init(NULL, NULL);
	// one of the important tasks that we have to establish is how many processes are in
	// this MPI instance. this will determine who and what we have to communicate with
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	// another important job is finding out which rank we are. we can use this rank
	// number to assign seperate jobs to different mpi units
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// depending on which rank we are call the appropriate function
	if (world_rank == 0)
		master();
	else
		slave();

	// before we can end our application we need to finalise everything so MPI can shut
	// down properly
	MPI_Finalize();
	// standard C/C++ thing to do at the end of main
	return 0;
}