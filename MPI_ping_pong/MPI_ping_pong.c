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
#include <stdlib.h>     
#include <time.h> 
#include <errno.h>      



const char* MSG = "blablabla";
const int BUFF_SIZE = 20;
const int EMPTY_PROC = -666;



int main(int argc, const char **argv)
{
    int error = 0;
    MPI_Init(NULL, NULL);

    int world_size;
    int world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_size % 2)
    {
        if(world_rank == 0)
            printf("Please enter number"
                   "of processes devisible by 2\n");

        MPI_Finalize();
        return 0;
    }

    int *proc_numbs = (int *)calloc(world_size, sizeof(int));
    if(!proc_numbs)
    {
        printf("Can't allocate memory, rank=%d, errno=%d",
               world_rank, errno);
        return errno;
    }
    
    for (int i = 0; i < world_size; i++)
        proc_numbs[i] = EMPTY_PROC;


    if(world_rank == 0)
    {
        srand(time(NULL));

        for(int i = 0; i < world_size; i++)
        {
            if(proc_numbs[i] != EMPTY_PROC)
                continue;

            int rand_pair = rand() % world_size;
            if((i == rand_pair) || (proc_numbs[rand_pair] != EMPTY_PROC))
            {
                i--;
                continue;
            }

            proc_numbs[i] = rand_pair;
            proc_numbs[rand_pair] = i;
        }

        printf("Random pairs:\n");
        for(int i = 0; i < world_size; i++)
            printf("%d : %d\n", i, proc_numbs[i]);

        for (int i = 0; i < world_size; i++)
        {
            error = MPI_Send(proc_numbs, world_size, MPI_INT,
                             i, 0, MPI_COMM_WORLD);
            if(error != MPI_SUCCESS) goto end;
        }
        printf("blablabla\n");
    }

    error = MPI_Recv(proc_numbs, world_size, MPI_INT, 0, 0,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if(error != MPI_SUCCESS) goto end;
    
    char *buff = (char *)calloc(BUFF_SIZE, sizeof(char));

    //  For deadlock avoiding
    if(world_rank < proc_numbs[world_rank])
    {
        error = MPI_Send(MSG, strlen(MSG), MPI_CHAR,
                 proc_numbs[world_rank], 0, MPI_COMM_WORLD);
        if(error != MPI_SUCCESS) goto end;
        printf("process %d sends *%s* to process %d\n",
               world_rank, MSG, proc_numbs[world_rank]);


        error = MPI_Recv(buff, BUFF_SIZE, MPI_CHAR,
                         proc_numbs[world_rank], MPI_ANY_TAG,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(error != MPI_SUCCESS) goto end;
        printf("process %d gets *%s* from process %d\n",
               world_rank, MSG, proc_numbs[world_rank]);
    }
    else
    {
        error = MPI_Recv(buff, BUFF_SIZE, MPI_CHAR,
                         proc_numbs[world_rank], MPI_ANY_TAG,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(error != MPI_SUCCESS) goto end;

        error = MPI_Send(buff, strlen(buff), MPI_CHAR,
                         proc_numbs[world_rank], 0, MPI_COMM_WORLD);
        if(error != MPI_SUCCESS) goto end;
    }

    free(buff);
    free(proc_numbs);

    error = 0;

end:
    MPI_Finalize();

    return error;
}
