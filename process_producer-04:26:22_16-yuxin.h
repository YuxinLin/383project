#ifndef PROCESS_PRODUCER
#define PROCESS_PRODUCER

#define NUM_PROCESS 50 //determine the number of process
#define MAX_QUANTA 100 //determine the maximum arrival_time
#define CHAR_ARRAYMAX 150

/* Structure: Process 
*For each simulated process, randomly generate:
*An arrival time: a float value from 0 through 99 (measured in quanta).
*An expected total run time: a float value from 0.1 through 10 quanta.
*A priority: integer 1, 2, 3, or 4 (1 is highest)
*/

struct process
{
	char name; // name from A to Z
	int priority; // 1 to 4, 1 is highest
	float arrival_time; // 0 to 99 
	float service_time; // 0.1 to 10
	int actual_start_time; //when the process is contacted by CPU for first time
	int end_time; //process finishes
        float initend_time;
};

void CreateProcesses(struct process** process_list);
/* Function: CreateNewProcess
*Constructs an array of struct process.
*The parameter process_list is a pointer that will point to the created list
*/

void PrintProcessList(struct process* a);
//Print all process properties includes name, ariority, arrival_time and aervice_time

#endif
