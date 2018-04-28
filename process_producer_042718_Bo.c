#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include "Sorter.h"

#define MAX_TABLE_SIZE 20

void PrintProcessList(struct process process_list[]){
	printf(" PID | Arrival | Start | Priority | Service | Response |  End  | Turnaround\n");
	for(int i = 0; i < NUM_PROCESS; i++){
		printf(" %3c | %7.1f | %5d | %8d | %7.1f | %8.1f | %5.1f | %7.1f \n", 
			process_list[i].name, process_list[i].arrival_time, process_list[i].actual_start_time, process_list[i].priority, 
			process_list[i].service_time, process_list[i].actual_start_time - process_list[i].arrival_time, 
			process_list[i].actual_start_time + process_list[i].service_time,
			process_list[i].actual_start_time + process_list[i].service_time - process_list[i].arrival_time);
	}
}

void printCharTable(char time_chart[]){
	printf("\n");
	for (int i = 0; i < CHAR_ARRAYMAX; i++) {
		printf("%c", time_chart[i]);
	}
	int j, k;
	for(j = 0; j< CHAR_ARRAYMAX; j = j+ MAX_TABLE_SIZE){
		printf("\n");
		printf(" Timeslice |");
		for(k = 0; j + k < CHAR_ARRAYMAX && k < MAX_TABLE_SIZE; k++)
        {
            printf(" %3d |", j+k);
        }
        printf("\n");
        printf("   Process |");
        for(k = 0; j + k < CHAR_ARRAYMAX && k < MAX_TABLE_SIZE; k++)
        {
            printf(" %3c |", time_chart[j+k]);
        }
        printf("\n");
	}
}

void CreateProcesses(struct process** process_list){
	char alphabet[60] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwx";
	float arrival_time; // [0 - 99]
	float service_time; // [0.1 - 10]
	int priority; // [1 - 4]
	*process_list = calloc(NUM_PROCESS, sizeof(struct process));
	 //generate proesses
	for(int i = 0; i < NUM_PROCESS; i++){
		arrival_time = rand() % 1000;
		arrival_time /= 10.0;
		service_time = rand() % (MAX_QUANTA - 1);
		priority = rand() % 5;
		if( service_time == 0 ) service_time = 1;
		service_time /= 10.0;
		if( priority == 0 ) priority = 1;

		(*process_list)[i].arrival_time = arrival_time;
		(*process_list)[i].service_time = service_time;
		(*process_list)[i].priority = priority;
		(*process_list)[i].name = alphabet[i];
		
		//init set start / end -1 -> indicate the process executed or not
		(*process_list)[i].actual_start_time = -1;
		(*process_list)[i].end_time = -1;
		(*process_list)[i].initend_time = arrival_time + service_time;
	}
}
