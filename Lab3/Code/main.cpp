#include <stdio.h>
#include "mpi.h"
#include "iostream"

int main(int argc, char *argv[])
{
    int rank;
    int size;
    int n, i;
    MPI_Status stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            printf("Err: There needs to be at least 2 processes\n");
        }
        MPI_Finalize();
        return 0;
    }

    n = 3 * size;

    if (rank == 0)
    {
        int *a = new int[n];
        for (i = 0; i < n; i++) {
            a[i] = i; 
        }

        printf("\trank= %d a: ", rank);
        for (i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        printf("\n----------------------------------------------------\n");

        for (i = 0; i < n; i++) {
            int target_process = i % size;

            if (target_process != 0) {
                MPI_Send(&a[i], 1, MPI_INT, target_process, 777, MPI_COMM_WORLD);
            }
        }

        delete[] a;
    }
    else
    {
        int count = 3;
        int *b = new int[count];

        for (i = 0; i < count; i++) {
            MPI_Recv(&b[i], 1, MPI_INT, 0, 777, MPI_COMM_WORLD, &stat);
        }

        printf("\trank= %d b: ", rank);
        for (i = 0; i < count; i++) {
            printf(" %d ", b[i]);
        }
        printf("\n");

        delete[] b;
    }

    MPI_Finalize();
    return 0;
}
