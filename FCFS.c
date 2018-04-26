#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_producer.h"
#define TRUE 1
#define FALSE 0

char* getFCFSOrder(struct process** plist, int* size) {

	int numberOfProcessCompleted = 0;
	char* arrayFCFSOrder = malloc(sizeof(char) * CHAR_ARRAYMAX); // Allocate enough to leave spaces for last process to finish 
	memset(arrayFCFSOrder, 32, sizeof(char) * CHAR_ARRAYMAX);
	int quanta = 0;
	*size = 0;
	for (int i = 0; i < NUM_PROCESS; i++) {

		while ((*plist)[i].arrival_time > quanta && (quanta < MAX_QUANTA)) { 
		// Keep looping because the process hasn't arrived
		// A new process cannot start if MAX_QUANTA is already hit
			arrayFCFSOrder[quanta] = ' ';
			quanta++; 
		}

		// After the first while loop, the current process is sure to be starting. This is the actual start time
		// of a process after the waiting of the previous
		(*plist)[i].actual_start_time = quanta;
		while (((*plist)[i].service_time  > 0)) { 
			arrayFCFSOrder[quanta] = (*plist)[i].name;
			quanta++;
			(*plist)[i].service_time = (*plist)[i].service_time - 1; // Decrement a service time each time it's been added to array of process done array (arrayFCFS)
		}

		// Actual end time after service time is finished
		(*plist)[i].end_time = quanta - 1;
		*size += 1;

		if (quanta > MAX_QUANTA - 1) {
			break;
		}
	}

	return arrayFCFSOrder;
}



