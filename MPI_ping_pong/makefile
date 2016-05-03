EXECS=MPI_ping_pong
MPICC?=mpicc
CFLAGS=-std=c99

all: ${EXECS}

MPI_ping_pong: MPI_ping_pong.c
	${MPICC} ${CFLAGS} -o MPI_ping_pong MPI_ping_pong.c

clean:
	rm -f ${EXECS}
