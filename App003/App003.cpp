/** simple program to test the MPI stuff to see if it works **/
/** includes **/
#include <iostream>
#include <cstdlib>
#include <mpi.h>

/** messages for communicating tasks **/
int COMPUTE_AVERAGE = 1;

/** the world rank and size that will be useful in many functions **/
int world_size;
int world_rank;

void coordinator(void) {
	int message;

	// tell the next node that we have to start computing an average
	MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

	// we will expect the last node to send us a message to compute our average. get it
	// and ignore it
	MPI_Recv(&message, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	// compute our average and print out the results
	srand(world_rank);
	int sum = 0;
	for (int i = 0; i < 100000; i++)
		sum += rand() % 10;
	float average = sum / 100000.f;
	std::cout << "coordinator 0 sum is: " << sum << std::endl;
	std::cout << "coordinator 0 average is: " << average << std::endl;

	// ask node one to compute it's average by sending a single floating point value this
	// will add in it's average and pass to node 2 etc
	// when we get the result back we will have the full average
	MPI_Send(&average, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
	MPI_Recv(&average, 1, MPI_FLOAT, world_size - 1, 0, MPI_COMM_WORLD,
		MPI_STATUS_IGNORE);

	// print out the average by dividing by the number of nodes and exit
	std::cout << "coordinator 0 total average is: " << average / world_size << std::endl;
}

void computeAverage(void) {
	int message;
	float current_average;

	// wait till we get the compute message after this tell the next node to start
	// computing an average before computing our average
	MPI_Recv(&message, 1, MPI_INT, (world_rank + world_size - 1) % world_size, 0,
		MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, (world_rank + 1) % world_size, 0,
		MPI_COMM_WORLD);
	srand(world_rank);
	int sum = 0;
	for (int i = 0; i < 100000; i++)
		sum += rand() % 10;
	float average = sum / 100000.f;

	// print out infomation about our sum and average
	std::cout << "node " << world_rank << " sum is: " << sum << std::endl;
	std::cout << "node " << world_rank << " average is: " << average << std::endl;

	// at some point we will get a message that will contain a current total of the
	// overall average. take that value add our average
	// to it and pass it on to the next node
	MPI_Recv(&current_average, 1, MPI_FLOAT, (world_rank + world_size - 1) % world_size,
		0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	current_average += average;
	std::cout << "node " << world_rank << " current average is " << current_average <<
		std::endl;
	MPI_Send(&current_average, 1, MPI_FLOAT, (world_rank + 1) % world_size, 0,
		MPI_COMM_WORLD);
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

	// compute the average of 400 numbers by using ring like communication. whoever is
	// node zero should be given the job of coordinator
	if (world_rank == 0)
		coordinator();
	else
		computeAverage();

	// before we can end our application we need to finalise everything so MPI can shut
	// down properly
	MPI_Finalize();

	// standard C/C++ thing to do at the end of main
	return 0;
}