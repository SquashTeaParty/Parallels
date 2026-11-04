/*
    compile:    mpic++ main.cpp
    execute:    mpiexec -n 4
*/

#include <stdio.h>
#include "mpi.h"
#include <iostream>

int main(int argc, char *argv[])
{
    int rank, size;
    int n = 5;
    MPI_Status stat;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        int **a = new int*[size];
        a[0] = new int[size * n];
        for (int i = 1; i < size; i++) {
            a[i] = a[i - 1] + n;
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = 10 * (i + 1) + j + 1;
            }
        }

        for (int i = 1; i < size; i++) {
            MPI_Send(a[i], n, MPI_INT, i, 777, MPI_COMM_WORLD);
        }

        printf("Rank = %d received row: ", rank);
        for (int j = 0; j < n; j++) {
            printf("%d ", a[0][j]);
        }
        printf("\n");

        delete[] a[0];
        delete[] a;
    } 
    else {
        int *b = new int[n];
        
        MPI_Recv(b, n, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);

        printf("Rank = %d received row: ", rank);
        for (int j = 0; j < n; j++) {
            printf("%d ", b[j]);
        }
        printf("\n");

        delete[] b;
    }

    MPI_Finalize();
    return 0;
}
