/*
    compile:    mpic++ main.cpp
    execute:    mpiexec -n 4 ./a.out
*/

#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 5;                                              // fixed row length
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);           // all processes sync

    int count = 2 * n;                                      // elements from this process
    int *a = new int[count];                                // local 2 x n
    for (int i = 0; i < count; i++) a[i] = rank;

    int *countPerProc = new int[size];                      // counts per process
    int *displace = new int[size];                          // displacements
    int offset = 0;
    for (int i = 0; i < size; i++) {
        countPerProc[i] = 2 * n;                            // same block each
        displace[i] = offset;
        offset += countPerProc[i];
    }

    int *total = new int[offset];                           // 2*size*n elements
    MPI_Gatherv(a, count, MPI_INT,
                total, countPerProc, displace, MPI_INT,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered:");                                // root prints result
        int pos = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < countPerProc[i]; j++) {
                if (j == 0) printf("   %d", total[pos]);
                else        printf(",%d", total[pos]);
                pos++;
            }
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}