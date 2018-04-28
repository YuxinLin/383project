#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_producer.h"
#include "FCFS.h"
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

#define SERVICE_TIME_UPPER_BOUND 10

//Revised the Turnaround time, Waiting time and response time
float avgTurnaround(struct process* list, int size){
	float totalTurnaround = 0.0;
	int finishedProcess=0;
	for(int i=0; i<size; i++){
		if(list[i].end_time > 0){  //[i] finished
			finishedProcess++;
			totalTurnaround += (list[i].actual_start_time + list[i].service_time - list[i].arrival_time);
		}
	}
	return finishedProcess > 0 ? totalTurnaround/finishedProcess : 0;
	//return totalTurnaround/finishedProcess;
}

float avgWaiting(struct process* list, int size){
	float totalWaiting = 0.0;
	int finishedProcess=0;
	for(int i=0; i<size; i++){
		if(list[i].end_time>0){
			finishedProcess++;
			totalWaiting += (list[i].actual_start_time - list[i].arrival_time);
		}
	}
	return finishedProcess > 0 ? totalWaiting/finishedProcess : 0;
}

float avgResponse(struct process* list, int size){
	float totalResponse = 0.0;
	int finishedProcess = 0;
	for(int i=0; i<size; i++){
		if(list[i].end_time>0){
			finishedProcess++;
			totalResponse += (list[i].actual_start_time - list[i].arrival_time);
		}
	}
	return finishedProcess > 0 ? totalResponse/finishedProcess : 0;
}


char* firstComeFirstServed(struct process* plist)
{
	int currQuantum = 0; //1; //0; -> actually the end time of curr quantum [0, 1] -> 1
	int num_finished_jobs = 0;
	
	char* time_chart = malloc(sizeof(char) * CHAR_ARRAYMAX); // Allocate enough to leave spaces for last process to finish 
	
	memset(time_chart, 32, sizeof(char) * CHAR_ARRAYMAX); //check memset -> 32 -> space
	struct process* jobs_queue = malloc(sizeof(struct process) * NUM_PROCESS); //initialize arrays with size of NUM_PROCESS
	memcpy(jobs_queue, plist, (sizeof(struct process) * NUM_PROCESS)); //copy processes into newArrivalArray
	
	
	bool finished[NUM_PROCESS]; //a bool array to indicate the job is finished or not
	for(int i = 0; i < NUM_PROCESS; i++) { //init -> false
		finished[i] = false;
	}
	
	bool done = false; //indicate simulation done or not
	while(!done) {
		//find the first unexecuted process
		int first_unexecuted_job_idx = 0;
		while (first_unexecuted_job_idx < NUM_PROCESS && finished[first_unexecuted_job_idx]) {
			first_unexecuted_job_idx++;
		}
		//idx == num_proc || finished[idx] == false
		if(first_unexecuted_job_idx == NUM_PROCESS) {
			break; //edge case: num_process == 0
		}
		//increment quantum until first job arrived
		while(currQuantum < jobs_queue[first_unexecuted_job_idx].arrival_time) {

			time_chart[currQuantum++] = '-'; //update time chart
		}
		//currQuantum >= queue[0].arrival -> at least one process arrived and not executed yet
		//find out the shortest one among all arrived jobs that not finished yet

		//shortest job -> the job to run found -> set the actual start time
		jobs_queue[first_unexecuted_job_idx].actual_start_time = currQuantum;
		float actual_end_time = currQuantum + jobs_queue[first_unexecuted_job_idx].service_time; //might not be an integer
		char curr_job_name = jobs_queue[first_unexecuted_job_idx].name;
		while (currQuantum < actual_end_time) {
			time_chart[currQuantum++] = curr_job_name; //print curr job name to record -> time chart
		}
		//currQuantum >= actual_end_time now
		//print curr job
		//PrintProcess(jobs_queue[shortest_idx]);
		jobs_queue[first_unexecuted_job_idx].end_time = currQuantum;
		finished[first_unexecuted_job_idx] = true; //update finished
		num_finished_jobs++; //increment num of finished jobs
		if(currQuantum >= MAX_QUANTA - 1 || num_finished_jobs == NUM_PROCESS) {
			done = true; //max quanta reached or no more job to run
		}
	}

	PrintProcessList(jobs_queue); //print list of processes
	printCharTable(time_chart);
	
	/*printf("\n");
	for (int i = 0; i < CHAR_ARRAYMAX; i++) {
		printf("%c", time_chart[i]);
	}*/
	
	printf("\n");

	printf("Average turnaround time: %.2f\n", avgTurnaround(jobs_queue, NUM_PROCESS));
	printf("Average response time: %.2f\n", avgResponse(jobs_queue, NUM_PROCESS));
	printf("Average waiting time: %.2f\n", avgWaiting(jobs_queue, NUM_PROCESS));
	printf("Throughput in 100 quanta: %.d\n", num_finished_jobs);

	free(jobs_queue); //don't forget to free the memory space for malloc
	
	return time_chart; //why return it?
}




