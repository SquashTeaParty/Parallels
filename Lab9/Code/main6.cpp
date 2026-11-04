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

    int count = size - rank;                                // elements from this process
    int *a = new int[count];
    for (int i = 0; i < count; i++) a[i] = rank;

    int *countPerProc = new int[size];                      // counts per process
    int *displace = new int[size];                          // displacements
    for (int i = 0; i < size; i++) {
        countPerProc[i] = size - i;
        displace[i] = i * size + i;                         // start on diagonal
    }

    int *total = new int[size * size];                      // size x size
    for (int i = 0; i < size * size; i++) total[i] = 0;     // zero padding

    MPI_Gatherv(a, count, MPI_INT,
                total, countPerProc, displace, MPI_INT,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Gathered:\n");                              // root prints matrix
        int pos = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (j == 0) printf("   %d", total[pos]);
                else        printf(",%d", total[pos]);
                pos++;
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}