LAD_ENV := $(shell command -v ladcomp 2> /dev/null)

S_DIR = ./src

CC = gcc
MPI = mpicc
LAD_CMD = ladcomp -env mpicc

CFLAGS = -Wall -g

all: sequential.o parallel_master.o parallel_slave.o

sequential.o: $(S_DIR)/sequential.c
	$(CC) -o $@ $< $(CFLAGS)

parallel_master.o: $(S_DIR)/parallel_master.c
ifdef LAD_ENV
	$(LAD_CMD) -o $@ $< $(CFLAGS)
else
	$(MPI) -o $@ $< $(CFLAGS)
endif

parallel_slave.o: $(S_DIR)/parallel_slave.c
ifdef LAD_ENV
	$(LAD_CMD) -o $@ $< $(CFLAGS)
else
	$(MPI) -o $@ $< $(CFLAGS)
endif
	
.PHONY: clean

clean:
	rm -rf *.o *~ all