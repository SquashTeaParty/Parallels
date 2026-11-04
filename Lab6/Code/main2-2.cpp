/*
    compile:    mpic++ main.cpp
    execute:    mpiexec -n 4
*/
#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank, size;
    int n = 1000000;
    double t1, t2, s = 0, s1;
    MPI_Status stat;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) t1 = MPI_Wtime();

    // каждый процесс считает свою частичную сумму
    for (int i = rank; i < n; i += size)
        s += 1.0 / (1.0 + i);

    if (rank == 0)
    {
        // 0-й процесс принимает строго по порядку: от 1, от 2, ...
        s1 = s;
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&s, 1, MPI_DOUBLE, i, 777,
                     MPI_COMM_WORLD, &stat);
            s1 += s;
        }
        t2 = MPI_Wtime();
        printf("NO_ANY  n=%d size=%d sum=%.10f time=%.6f\n",
               n, size, s1, t2 - t1);

        // дописываем результат в конец файла
        FILE *f = fopen("results/task2_noany.txt", "a");
        if (f != NULL)
        {
            fprintf(f, "size=%d \tsum=%.10f\ttime=%.6f\n",
                    size, s1, t2 - t1);
            fclose(f);
        }
    }
    else
    {
        MPI_Send(&s, 1, MPI_DOUBLE, 0, 777, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}