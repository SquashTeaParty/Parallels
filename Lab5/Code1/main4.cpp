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
    int n = 4; 

    int rows_per_proc = 3;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    

    int **b = new int*[rows_per_proc];
    b[0] = new int[rows_per_proc * n];
    for (int i = 1; i < rows_per_proc; i++) {
        b[i] = b[i - 1] + n;
    }
    

    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < n; j++) {
            b[i][j] = 100 * (rank + 1) + 10 * (i + 1) + j + 1;
        }
    }
    if (rank == 0) {
        int total_rows = size * rows_per_proc;
        int **a = new int*[total_rows];
        a[0] = new int[total_rows * n];
        for (int i = 1; i < total_rows; i++) {
            a[i] = a[i - 1] + n;
        }
        

        for (int i = 0; i < rows_per_proc; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = b[i][j];
            }
        }
        

        for (int i = 1; i < size; i++) {
            int start_row = i * rows_per_proc;
            MPI_Recv(a[start_row], rows_per_proc * n, MPI_INT, i, 777, MPI_COMM_WORLD, &stat);
        }
        

        printf("\n\t--- Rank 0 result ---\t\n");
        for (int i = 0; i < total_rows; i++) {
            if (i % rows_per_proc == 0)
            printf("From Rank %d:\n", i / rows_per_proc); printf("\t");
            for (int j = 0; j < n; j++) {
                printf("%3d ", a[i][j]);
            }
            printf("\n");
        }
        delete[] a[0];
        delete[] a;
    } 
    else {
        

        MPI_Send(b[0], rows_per_proc * n, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }
    delete[] b[0];
    delete[] b;
    MPI_Finalize();
    return 0;
}
