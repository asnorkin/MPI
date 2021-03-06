//*********************************************************
//********                                       **********
//********      Created by Alexander Snorkin     **********
//********              02.05.2016               **********
//********                                       **********
//*********************************************************
#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>



const char* MSG = "blablabla";
const int BUFF_SIZE = 20;



int main(int argc, const char **argv)
{
    MPI_Init(NULL, NULL);

    int world_size;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    char *buff = (char *)calloc(buff_size, sizeof(char));
    if(!buff)
    {
	printf("Can't allocate memory, rank=%d, errno=%d", world_rank, errno);
	return errno;
    }

    int error = 0;
    if(world_rank == 0)
    {
        error = MPI_Send(MSG, strlen(MSG) + 1, MPI_CHAR,
                 world_rank+1, 0, MPI_COMM_WORLD);
	if(error != MPI_SUCCESS)
	{
	    printf("MPI_Send has failed: rank= %d, error=%d",
			 world_rank, error);
	    return error;    
	}

        printf("processor %d out of %d sent *%s*\n",
		 world_rank, world_size, msg);
    }
    else if(world_rank == world_size-1)
    {
        error = MPI_Recv(buff, BUFF_SIZE, MPI_CHAR,
                 MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if(error != MPI_SUCCESS)
	{
	    printf("MPI_Recv has failed: rank=%d, error=%d",
		 world_rank, error);
	    return error;    
	}

        printf("processor %d out of %d get *%s*\n",
		 world_rank, world_size, buff);
    }
    else if((world_rank != 0) && (world_rank != world_size-1))
    {
        MPI_Recv(buff, BUFF_SIZE, MPI_CHAR,
                 MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if(error != MPI_SUCCESS)
	{
	    printf("MPI_Recv has failed: rank=%d, error=%d",
		 world_rank, error);
	    return error;    
	}
        printf("processor %d out of %d get *%s*\n",
		 world_rank, world_size, buff);

        MPI_Send(buff, strlen(buff), MPI_CHAR,
                 world_rank+1, 0, MPI_COMM_WORLD);
	if(error != MPI_SUCCESS)
	{
	    printf("MPI_Send has failed: rank=%d, error=%d",
		 world_rank, error);
	    return error;    
	}
        printf("processor %d out of %d sent *%s*\n",
		 world_rank, world_size, buff);
    }
  
    free(buff);  

    MPI_Finalize();

    return 0;
}
