/*
    compile:    mpic++ main.cpp
    execute:    mpiexec -n 4 ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank, size;
    int count, i, j;
    MPI_Status stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        // Matrix to collect rows from all processes
        int **a = new int *[size];
        a[0] = new int[size * size];
        for (i = 1; i < size; i++)
            a[i] = a[i - 1] + size;

        // Initialize with zeros
        for (i = 0; i < size; i++)
            for (j = 0; j < size; j++)
                a[i][j] = 0;

        // Receive from each non-zero process
        for (i = 1; i < size; i++)
        {
            // Probe to get message info
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
            // Get number of elements
            MPI_Get_count(&stat, MPI_INT, &count);
            // Receive into corresponding row
            MPI_Recv(a[stat.MPI_SOURCE], count, MPI_INT,
                     stat.MPI_SOURCE, 777, MPI_COMM_WORLD, &stat);
            printf("source = %d\tcount = %d\n", stat.MPI_SOURCE, count);
        }

        // Print resulting matrix
        printf("rank = %d \ta:\n", rank);
        for (i = 0; i < size; i++)
        {
            printf("\t\t");
            for (j = 0; j < size; j++)
                printf("%d ", a[i][j]);
            printf("\n");
        }
    }
    else
    {
        // Random length from 1 to size
        int len = rand() % size + 1;
        int *a = new int[len];

        // Fill with rank values
        for (i = 0; i < len; i++)
            a[i] = rank;

        // Print sent array
        printf("rank=%d\t\tsent:\t", rank);
        for (i = 0; i < len; i++)
            printf(" %d", a[i]);
        printf("\n");

        // Send array
        MPI_Send(a, len, MPI_INT, 0, 777, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}