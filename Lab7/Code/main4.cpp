/*
    compile:    mpic++ main.cpp
    execute:    mpiexec -n 4 ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void test_transmission(int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    MPI_Status stat;
    double t1, t2;
    double result1, result2;

    int *sended     = new int[n];
    int *recieved   = new int[n];

    for (int i = 0; i < n; i++) {
        sended[i]   = rank * 10 + (i % 10);
        recieved[i] = -1;
    }

    // Not combined
    t1 = MPI_Wtime();
    if (rank == 0) {
        MPI_Send(sended, n, MPI_INT, 1, 111, MPI_COMM_WORLD);
        MPI_Recv(recieved, n, MPI_INT, 1, 222, MPI_COMM_WORLD, &stat);
    } else if (rank == 1) {
        MPI_Recv(recieved, n, MPI_INT, 0, 111, MPI_COMM_WORLD, &stat);
        MPI_Send(sended, n, MPI_INT, 0, 222, MPI_COMM_WORLD);
    }
    t2 = MPI_Wtime();

    result1 = t2 - t1;

    // Reset
    for (int i = 0; i < n; i++) recieved[i] = -1;

    // ---------------------------------------------------------------------

    // Combined
    t1 = MPI_Wtime();
    if (rank == 0) {
        MPI_Sendrecv(sended, n, MPI_INT, 1, 333,
                     recieved, n, MPI_INT, 1, 444,
                     MPI_COMM_WORLD, &stat);
    } else if (rank == 1) {
        MPI_Sendrecv(sended, n, MPI_INT, 0, 444,
                     recieved, n, MPI_INT, 0, 333,
                     MPI_COMM_WORLD, &stat);
    }
    t2 = MPI_Wtime();

    result2 = t2 - t1;

    printf("\t[Size: %d elements]  \tProcess %d -> send/recv time: %.6f sec \t| sendrecv time: %.6f sec\n", 
           n, rank, result1, result2);

    delete[] sended;
    delete[] recieved;
}

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("Only 2 processes are needed.\n");
        }
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        printf("Results:\n");
    }

    // Test
    test_transmission(10000);
    test_transmission(100000);
    test_transmission(1000000);

    MPI_Finalize();
    return 0;
}
