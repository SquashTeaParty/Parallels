#include <stdio.h>
#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Status stat;
    int rank;
    int size;
    int n;
    double a=0,t1,t2,t,Mflops;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cin>>n;
    t1=MPI_Wtime();

    for(int i=0;i<n;i++)
        a=a+0.000000000001+0.000000000001;

    t2=MPI_Wtime();

    t=(t2-t1)/(2*n);

    Mflops=1/t/1000000;

    cout<<"n= "<<n<<" T = "<<t2-t1<<" t="<<t<<" Mflops= "<<Mflops<<endl;

    MPI_Finalize();
    return 0;
}