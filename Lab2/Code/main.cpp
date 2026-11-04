#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int a = 0;

    if (rank == 0) {
        a = 100;
        std::cout << "Rank 0 started with a = " << a << std::endl;
        
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        std::cout << "Rank 0 received final back a = " << a << std::endl;
    } else {
        MPI_Recv(&a, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        a += 1;
        int next_rank = (rank + 1) % size;
        
        MPI_Send(&a, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
