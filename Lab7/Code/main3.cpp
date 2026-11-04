/*
    compile:    mpic++ main.cpp
    execute:    mpiexec -n 4 ./a.out
*/

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Status stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            printf("At least 2 processes are needed.\n");
        }
        MPI_Finalize();
        return 0;
    }

    int dest, source;
    
    if (rank % 2 == 0) {
        // Even
        int total_evens = (size + 1) / 2;
        int local_idx   = rank / 2;
        
        dest    = ((local_idx + 1) % total_evens) * 2;
        source  = ((local_idx - 1 + total_evens) % total_evens) * 2;
    } else {
        // Odd
        int total_odds  = size / 2;
        int local_idx   = (rank - 1) / 2;
        
        dest    = ((local_idx + 1) % total_odds) * 2 + 1;
        source  = ((local_idx - 1 + total_odds) % total_odds) * 2 + 1;
    }

    int send_data = rank;
    int recv_data = -1;

    MPI_Sendrecv(&send_data, 1, MPI_INT, dest, 777,
                 &recv_data, 1, MPI_INT, source, 777,
                 MPI_COMM_WORLD, &stat);

    printf("Process %d (ring: %s)\t-> Sent: %d, Received from %d: %d\n", 
           rank, (rank % 2 == 0 ? "EVEN" : "ODD"), send_data, source, recv_data);

    MPI_Finalize();
    return 0;
}