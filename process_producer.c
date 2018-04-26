#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include "Sorter.h"

void PrintProcessList(struct process process_list[]){
	for(int x = 0; x < NUM_PROCESS; x++){
		printf("%c: Service Time: %.1f Priority: %02d Arrival Time: %02d\n", process_list[x].name, process_list[x].service_time, process_list[x].priority, process_list[x].arrival_time);
	}
}

void CreateProcesses(struct process** process_list){
	char alphabet[36] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
	float arrival_time; // [0 - 99]
	float service_time; // [0.1 - 10]
	int priority; // [1 - 4]
	*process_list = calloc(NUM_PROCESS, sizeof(struct process));
	 //generate proesses
	for(int x = 0; x < NUM_PROCESS; x++){
		arrival_time = rand() % MAX_QUANTA;
		service_time = rand() % (MAX_QUANTA - 1);
		priority = rand() % 5;
		if( service_time == 0 ) service_time += 1;
		service_time = service_time / 10.0;
		if( priority == 0 ) priority += 1;

		(*process_list)[x].arrival_time = arrival_time;
		(*process_list)[x].service_time = service_time;
		(*process_list)[x].priority = priority;
		(*process_list)[x].name = alphabet[x];
	}
}

/*
Deprecated, use SortBy in Sorter.h instead
void SortProcesses(struct process** process_list) {
	int temp;

	for (int x = 0; x < NUM_PROCESS; x++) {
		for (int y = x + 1; y < NUM_PROCESS; y++) {

			if ((*process_list)[y].arrival_time < (*process_list)[x].arrival_time) {
				temp = (*process_list)[x].arrival_time;
				(*process_list)[x].arrival_time = (*process_list)[y].arrival_time;
				(*process_list)[y].arrival_time = temp;
			}

		}
	}

}
*/