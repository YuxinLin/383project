#ifndef PROCESS_PRODUCER
#define PROCESS_PRODUCER

#define NUM_PROCESS 50 //50 max possible number
#define MAX_QUANTA 100
#define CHAR_ARRAYMAX 150

/* Struct: process
*  struct created in accordance to Ezzat's specifications
*/
struct process{
	char name; // name A-Z or 0 - 9 or a-x
	int priority; // priority 1 - 4, 1 highest priority for priority based scheduling
	float arrival_time; // time process arrives in queue
	float service_time; // how much time to finish process
	int actual_start_time; // when the process is contacted by CPU for first time
	int end_time; // process finishes
	float initend_time; // initial end time, used for SRT (AG), it will be changed during the running SRT
};

/**
 * [PrintProcessList - prints all process attributes except end time and actual start time]
 * @param a [list to print]
 */
void PrintProcessList(struct process process_list[]);

void printCharTable(char time_chart[]);

/**
 * [CreateProcesses pass in a pointer, this function constructs an array of struct process. How many process is determined by the macro NUM_PROCESS and the max arrival time is determined by MAX_QUANTA]
 * @param process_list [pointer that will point to the created list]
 */
void CreateProcesses(struct process** process_list);
//void SortProcesses(struct process** process_list);

#endif