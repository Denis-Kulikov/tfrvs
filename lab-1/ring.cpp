#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int msg_size = 1024 * 1024;
    char* send_buf = new char[msg_size];
    char* recv_buf = new char[msg_size];

    MPI_Request send_req, recv_req;
    MPI_Status status;

    double start_time = MPI_Wtime();
    for (int i = 0; i < 1000; ++i) {
        MPI_Isend(send_buf, msg_size, MPI_CHAR, (rank + 1) % size, 0, MPI_COMM_WORLD, &send_req);
        MPI_Irecv(recv_buf, msg_size, MPI_CHAR, (rank - 1 + size) % size, 0, MPI_COMM_WORLD, &recv_req);
        MPI_Wait(&send_req, &status);
        MPI_Wait(&recv_req, &status);
    }
    double end_time = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Average communication time: " << (end_time - start_time) / 1000 << " seconds" << std::endl;
    }

    delete[] send_buf;
    delete[] recv_buf;

    MPI_Finalize();
    return 0;
}
