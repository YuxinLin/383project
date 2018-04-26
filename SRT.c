#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include <stdbool.h>


char* SRT(struct process* plist) 
{
	// initialize variables
	int numberOfProcessCompleted = 0;
	char *arraySRTOrder = malloc(sizeof(char) * CHAR_ARRAYMAX);
	char temp;
	int quanta = 0;
	int i = 0;

	//take the number of processes that can be completed within the alloted quanta and stores the value in variable i.
	for (; i < sizeof(*plist)/sizeof(*plist[0]; i++))
	{
		quanta +=(*p.list)[i].service_time;
		if (quanta >= MAX_QUANTA - 1)
			break;
	}

	quanta = 0;	// set quanta zero again to start the computations

	struct process* current = malloc(sizeof(struct process[0]) * NUM_PROCESS);
	current = *plist[0];	// helper pointer for the struct
	
	// keep running the processes untill all of them have been completed.
	while (!checkPCompletion(*plist, i))
	{
		for (int j = i; j >= 0; j--)) // decreasing j to maintain fcfs for processes with equal service time
			if ((*plist)[j].arrival_time <= quanta && (*plist)[j].service_time < current.service_time)
				current = (*plist)[j];
		arraySRTOrder[quanta] = current.name;
		quanta++;
		current.service_time -=1;
	}

	//making the string ready for output by replacing consecutive process names by ' '
	temp = arraySRTOrder[0];
	for (int j = 1; j < sizeof(arraySRTOrder)/ sizeof(arraySRTOrder[0]); j++)
	{
		if (temp == arraySRTOrder[j])
			arraySRTOrder[j] = ' ';
		else
			temp = arraySRTOrder[j];
	}

	return arraySRTOrder;
}

bool checkPCompletion(struct process* plist, int i)
{
	for (int j = 0; j < i; j++)
		if ((*plist)[j] != 0)
			return false;
	return true;
}

