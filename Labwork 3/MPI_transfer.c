#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define BUFFER_SIZE 1024 * 10

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        printf("This program requires exactly 2 processes (1 client and 1 server).\n");
        MPI_Finalize();
        return 1;
    }

    char buffer[BUFFER_SIZE];
    FILE *file;

    if (rank == 0) {
        int server_rank = 1;

        file = fopen("test.txt", "r");
        if (file == NULL) {
            printf("[-] Error opening the file.\n");
            MPI_Finalize();
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);

        fread(buffer, 1, file_size, file);
        fclose(file);

        MPI_Send(buffer, file_size, MPI_CHAR, server_rank, 0, MPI_COMM_WORLD);
        printf("[Client] File sent successfully.\n");

    } else if (rank == 1) {
        MPI_Status status;
        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        int count;
        MPI_Get_count(&status, MPI_CHAR, &count);

        MPI_Recv(buffer, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        file = fopen("received_file.txt", "w");

        fwrite(buffer, 1, count, file);
        fclose(file);

        printf("[Server] Data received and written to 'received_file.txt' successfully.\n");
    }

    MPI_Finalize();
    return 0;
}
