EXECS=MPI_circle
MPICC?=mpicc

all: ${EXECS}

MPI_circle: MPI_circle.c
	${MPICC} -o MPI_circle MPI_circle.c

clean:
	rm -f ${EXECS}
