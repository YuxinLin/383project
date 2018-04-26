CC=gcc
HDIR=./include
CFLAGS=-I$(HDIR)	-std=c99	-g
makeHW2:	process_producer.o	main.o FCFS.o RR.o	Sorter.o Calculation.o 	PreHFPA.o 	HPFA.o ShortestJobFirst.o
	$(CC)	$(CCFLAGS)	-o	producer process_producer.o	main.o	FCFS.o RR.o	Sorter.o Calculation.o 	PreHFPA.o 	HPFA.o ShortestJobFirst.o

clean:
	rm	-f	*.o	makeHW2