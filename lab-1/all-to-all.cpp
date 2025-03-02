#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main (int argc, char **argv)
{
	int rank, size, len;
 	char procname[MPI_MAX_PROCESSOR_NAME];
 	MPI_Init(&argc, &argv);
 	MPI_Get_processor_name(procname, &len);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Request reqs[(size -1) * 2];
    MPI_Status stats[(size -1) * 2];
	int count = 1024 * 128;
    char *rbuf = (char*)malloc(sizeof(*rbuf) * count * size);
    char *sbuf = (char*)malloc(sizeof(*sbuf) * count);
    double total = 0;

    const int N = 16;
    for (int j = 0; j < N; ++j) {
        double t = MPI_Wtime();
        for (int i = 0; i < size; i++) {
            if (i < rank) {
                MPI_Isend(sbuf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[i * 2]);
                MPI_Irecv(rbuf + count * i, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[i * 2 + 1]);
            } else if (i > rank) {
                MPI_Isend(sbuf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[(i - 1) * 2]);
                MPI_Irecv(rbuf + count * i, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &reqs[(i - 1) * 2 + 1]);
            }
        }
        MPI_Waitall((size -1) * 2, reqs, stats);
        total += MPI_Wtime() - t;
    }

    if (rank == 0) printf("%f\n", total / N);

	MPI_Finalize();

	return 0;
}
