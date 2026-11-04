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

    int a[3];
    for (int i = 0; i < 3; i++) a[i] = rank;    // fill local array

    int *s = new int[3 * size];                 // receive buffer on root
    MPI_Gather(a, 3, MPI_INT, s, 3, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {  
        printf("Gathered:");                  // root prints result
        for (int i = 0; i < 3 * size; i++) {
            if( i % 3 == 0) {
                printf("\t%d", s[i]); 
            } else { printf(",%d", s[i]); }
            
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}