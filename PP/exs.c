#include <stdio.h>
#include <mpi.h>
int main(int argc, char** argv)
{

    int size=0;
    int rank=0; 
    int buf = 0;
    MPI_Init(&argc, &argv);
    
    // int array[5];
    // int answer[5];

    double t;
     t = MPI_Wtime();
    // int i = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // if(rank!=size-1)
    //     MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, NULL);
    // printf("%d ", rank);
    if(rank!= 0) {
        MPI_Recv(&buf, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
        sleep(buf);
    }
    if(rank!= size-1) {
        MPI_Send(&rabk, 1, MPI_INT, (rank+1),0,MPI_COMM_WORLD);
    }
    // MPI_Barrier(MPI_COMM_WORLD);
   
    // int buf[1];
    // MPI_Status status;
    // if (rank!=0)
    //     MPI_Recv(buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    // sleep(1);
    // MPI_Send(buf, 1, MPI_INT, (rank+1)%size,0,MPI_COMM_WORLD);
    // sleep(rank);
    // MPI_Recv(buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    // sleep(1);
    // if(rank!=size-1)
    //     MPI_Send(buf, 1, MPI_INT, (rank+1)%size,0,MPI_COMM_WORLD);
    // MPI_Barrier(MPI_COMM_WORLD);
    // if(rank==0)
    // printf("%f\n\n", MPI_Wtime() - t);
    // int buf[7] = {10, 20, 30, 40, 50, 60, 70};
// int recv_buf[3];
// MPI_Scatter(buf, 3, MPI_INT, recv_buf, 3, MPI_INT, 0, MPI_COMM_WORLD);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Allgather(&rank, 1, MPI_INT, array, 1, MPI_INT, MPI_COMM_WORLD);
    
    //     for (i = 0; i < size; i++){
    //         array[i] -= rank;
    //     }
    
    // MPI_Reduce(array, answer, size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // // if (rank != 0){
    // //     MPI_Recv(&buf, 1, MPI_INT, rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
    // //     sleep(buf);
    // // }
    // // if (rank!=size-1) {
    // //     MPI_Send(&rank, 1, MPI_INT, rank+1,0,MPI_COMM_WORLD);
    // // }
    MPI_Barrier(MPI_COMM_WORLD);
    // if (rank == 0)
    // for (i = 0; i < size; i++)
    //     printf("%d ", answer[i]);
    MPI_Finalize();
    printf("%f\n\n", MPI_Wtime() - t);
    // Do something here
    

    return 0;
}