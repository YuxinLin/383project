#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include "sort.h"

void PrintProcessesList(struct process process_list[]){
	for(int i = 0; i < NUM_PROCESS; i++){
		printf("%c: Priority: %02d Arrival Time: %02d Service Time: %.1f\n", process_list[i].name, process_list[i].priority, process_list[i].arrival_time, process_list[i].service_time);
	}
}

void CreateProcesses(struct process** process_list){
	char alphabet[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //26 processes
	int priority; // 1-4
	float arrival_time; // 0-99
	float service_time; // 0.1-10

	*process_list = calloc(NUM_PROCESS, sizeof(struct process));

	for(int i = 0; i < NUM_PROCESS; i++){
		arrival_time = rand() % MAX_QUANTA;
		service_time = rand() % (MAX_QUANTA - 1);
		priority = rand() % 5;
		if( service_time == 0 ) service_time += 1;
		service_time = service_time / 10.0;
		if( priority == 0 ) priority += 1;

		process_list[i]->name = alphabet[i];
		process_list[i]->priority = priority;
		process_list[i]->arrival_time = arrival_time;
		process_list[i]->service_time = service_time;
	}
}