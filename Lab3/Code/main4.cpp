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

    n = rank + 1;

    if (rank == 0)
    {
        int t = 0; // total
        for (i = 1; i < size; i++) {
            t += (i + 1);
        }

        int *res = new int[t]; // result
        int cur = 0; // current

        for (i = 1; i < size; i++) {
            int rec = i + 1; // recvest

            MPI_Recv(&res[cur], rec, MPI_INT, i, 777, MPI_COMM_WORLD, &stat);
            
            cur += rec;
        }

        printf("\trank= %d a: ", rank);
        for (i = 0; i < t; i++) {
            printf("%d", res[i]);
            if (i < t - 1) {
                printf(",");
            }
        }
        printf("\n");
        // printf("\n----------------------------------------------------\n");

        delete[] res;
    }
    else
    {

        int *b = new int[n];

        for (i = 0; i < n; i++) {
            b[i] = rank;
        }

        MPI_Send(b, n, MPI_INT, 0, 777, MPI_COMM_WORLD);

        delete[] b;
    }

    MPI_Finalize();
    return 0;
}
