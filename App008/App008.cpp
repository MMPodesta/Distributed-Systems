/** simple program to show how the MPI broadcast function works **/
/** includes **/
#include <iostream>
#include <mpi.h>

// function that will implement the coordinator job of this application
void coordinator(int world_size) {
	std::cout << "coordinator rank 0 starting " << std::endl;

	// generate 100000 random integers and store them in an array
	int values[100000];
	for (unsigned int i = 0; i < 100000; i++)
		values[i] = rand() % 10;

	// determine the size of each partition by dividing 100000 by the world size
	// it is impertative that the world_size divides this evenly
	int partition_size = 100000 / world_size;
	std::cout << "coordinator rank 0 partition size is " << partition_size << std::endl;

	// broadcast the partition size to each node so they can setup up memory as appropriate
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	std::cout << "coordinator rank 0 broadcasted partition size" << std::endl;

	// send out a partition of data to each node
	for (unsigned int i = 1; i < world_size; i++) {
		MPI_Send(values + partition_size * i, partition_size, MPI_INT, i, 0, MPI_COMM_WORLD);
		std::cout << "coordinator rank 0 sent partition to rank " << i << std::endl;
	}

	// generate an average for our partition
	int total = 0;
	for (unsigned int i = 0; i < partition_size; i++)
		total += values[i];
	float average = (float)total / partition_size;
	std::cout << "coordinator rank 0 average is " << average << std::endl;

	// call a reduce operation to get the total average and then divide that by the world size
	float total_average = 0;
	MPI_Reduce(&average, &total_average, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	std::cout << "total average is " << total_average / world_size << std::endl;
}

// function that will implement the participant job of this applicaiton
void participant(int world_rank, int world_size) {
	std::cout << "participant rank " << world_rank << " starting" << std::endl;

	// get the partition size from the root and allocate memory as necessary
	int partition_size = 0;
	MPI_Bcast(&partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	std::cout << "participant rank " << world_rank << " recieved partition size of " <<
		partition_size << std::endl;

	// allocate the memory for our partition
	int* partition = new int[partition_size];

	// recieve the partition from the root
	MPI_Recv(partition, partition_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	std::cout << "participant rank " << world_rank << " recieved partiton from root" <<
		std::endl;

	// generate an average for our partition
	int total = 0;
	for (unsigned int i = 0; i < partition_size; i++)
		total += partition[i];
	float average = (float)total / partition_size;
	std::cout << "participant rank " << world_rank << " average is " << average << std::endl;

	// call a reduce operation to get the total average and then divide that by the world size
	float total_average = 0;
	MPI_Reduce(&average, &total_average, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	// as we are finished with the memory we should free it
	delete partition;
}

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

	// if we have a rank of zero then we are the coordinator. if not we are a participant
	if (world_rank == 0)
		coordinator(world_size);
	else
		participant(world_rank, world_size);

	// finalise the MPI library
	MPI_Finalize();
	return 0;
}