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
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int *b = new int[n];
    for (int j = 0; j < n; j++) {
        b[j] = 10 * (rank + 1) + j + 1; 
    }
    if (rank == 0) {
        
        int **a = new int*[size];
        a[0] = new int[size * n];
        for (int i = 1; i < size; i++) {
            a[i] = a[i - 1] + n;
        }
        
        for (int j = 0; j < n; j++) {
            a[0][j] = b[j];
        }
        
        for (int i = 1; i < size; i++) {
            
            MPI_Recv(a[i], n, MPI_INT, i, 777, MPI_COMM_WORLD, &stat);
        }
        
        printf("\n\t--- Rank 0 result ---\t\n");
        for (int i = 0; i < size; i++) {
        printf("Rank = %d received row: ", i, i);
            for (int j = 0; j < n; j++) {
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
        delete[] a[0];
        delete[] a;
    } 
    else {
        
        MPI_Send(b, n, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }
    delete[] b;
    MPI_Finalize();
    return 0;
}
