#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_producer.h"
#define TRUE 1
#define FALSE 0

char* getHPFOrder(struct process** plist, int* size)
{
	int numberOfProcessCompleted = 0;
	char* arrayHPFOrder = malloc(sizeof(char) * CHAR_ARRAYMAX); // Allocate enough to leave spaces for last process to finish
	memset(arrayHPFOrder, 32, sizeof(char) * CHAR_ARRAYMAX);
	int quanta = 0;
	struct process tempSwitch;
	*size = 0;
	for (int i = 0; i < NUM_PROCESS; i++)
        {





		while ((*plist)[i].arrival_time > quanta && (quanta < MAX_QUANTA))
            {
		// Keep looping because the process hasn't arrived
		// A new process cannot start if MAX_QUANTA is already hit
			arrayHPFOrder[quanta] = ' ';
			quanta++;
		}

		// After the first while loop, the current process is sure to be starting. This is the actual start time
		// of a process after the waiting of the previous
		(*plist)[i].actual_start_time = quanta;

		for (int j = 0; i < NUM_PROCESS; j++)
        {
            if((*plist)[j].priority == (*plist)[j+1].priority)
            {
                if ((*plist)[j+1].arrival_time <= (*plist)[j].arrival_time)
                {
                    tempSwitch = (*plist)[j];
                    (*plist)[j] = (*plist)[j+1];
                    (*plist)[j+1] = tempSwitch;
                }
            }
        }
		while (((*plist)[i].service_time  > 0))
            {
			arrayHPFOrder[quanta] = (*plist)[i].name;
			quanta++;
			(*plist)[i].service_time = (*plist)[i].service_time - 1; // Decrement a service time each time it's been added to array of process done array (arrayFCFS)
		}

		// Actual end time after service time is finished
		(*plist)[i].end_time = quanta - 1;
		*size += 1;

		if (quanta > MAX_QUANTA - 1)
            {
			break;
		}
	}

	return arrayHPFOrder;
}
