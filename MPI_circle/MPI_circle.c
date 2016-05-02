//*********************************************************
//********                                       **********
//********      Created by Alexander Snorkin     **********
//********              02.05.2016               **********
//********                                       **********
//*********************************************************
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <mpi.h>


const char* msg = "blablabla";
const int buff_size = 20;



int main(int argc, const char **argv)
{
    MPI_Init(NULL, NULL);

    int world_size;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char *buff = (char *)calloc(buff_size, sizeof(char));

    if(world_rank == 0)
    {
        MPI_Send(msg, strlen(msg) + 1, MPI_CHAR,
                 world_rank+1, 0, MPI_COMM_WORLD);
        printf("processor %d out of %d sent *%s*\n",
		 world_rank, world_size, msg);
    }
    else if(world_rank == world_size-1)
    {
        MPI_Recv(buff, buff_size, MPI_CHAR,
                 MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("processor %d out of %d get *%s*\n",
		 world_rank, world_size, buff);
    }
    else if((world_rank != 0) && (world_rank != world_size-1))
    {
        MPI_Recv(buff, buff_size, MPI_CHAR,
                 MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("processor %d out of %d get *%s*\n",
		 world_rank, world_size, buff);

        MPI_Send(buff, strlen(buff), MPI_CHAR,
                 world_rank+1, 0, MPI_COMM_WORLD);
        printf("processor %d out of %d sent *%s*\n",
		 world_rank, world_size, buff);
    }
  
    free(buff);  

    MPI_Finalize();

    return 0;
}
