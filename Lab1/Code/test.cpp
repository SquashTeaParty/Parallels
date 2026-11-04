#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> a(size);
    for (int i = 0; i < size; ++i) {
        a[i] = i;
    }

    int k = rank + 1;
    long long sum = 0;
    for (int i = 0; i < k; ++i) {
        sum += a[i];
    }

    std::cout << "Rank: " << rank << " k: " << k << " Sum: " << sum << std::endl;

    MPI_Finalize();
    return 0;
}
