EXECS=MPI_hello_world
MPICC?=mpicc

all: ${EXECS}

MPI_hello_world: MPI_hello_world.c
	${MPICC} -o MPI_hello_world MPI_hello_world.c

clean:
	rm -f ${EXECS}
