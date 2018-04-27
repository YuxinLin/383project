#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include "Calculation.h"
#include "ShortestJobFirst.h"
#include <string.h>
#include "Sorter.h"
#include <stdbool.h>

/*
 *revision history
 *not use local variable time_chart_idx as it's the same as curr_quantum
 *for(int i = first_unexecuted_job_idx; i < NUM_PROCESS; i++) { //i = 0 -> i = first_unexecuted_job_idx
 *delete tat / wait / response time functions
 *add PrintProcess(struct process curr_job) function
 *free(jobs_queue); -> move to after print stats!
 */

#define SERVICE_TIME_UPPER_BOUND 10

void PrintProcess(struct process curr_job){
	printf("%c: Arrival Time: %.1f Start Time: %2d Service Time: %.1f End Time: %.1f Turnaround Time: %.1f Response Time: %.1f\n", curr_job.name, curr_job.arrival_time, curr_job.actual_start_time, curr_job.service_time, curr_job.actual_start_time + curr_job.service_time, curr_job.actual_start_time + curr_job.service_time - curr_job.arrival_time, curr_job.actual_start_time - curr_job.arrival_time);
}

//jobs_list is an array of all the processes with info about arrival time and service time.
char* shortestJobFirst(struct process* jobs_list)
{
	int curr_quantum = 0; //1; //0; -> actually the end time of curr quantum [0, 1] -> 1
	int num_finished_jobs = 0;
	
	char* time_chart = malloc(sizeof(char) * CHAR_ARRAYMAX); // Allocate enough to leave spaces for last process to finish 
	//int time_chart_idx = 0;
	memset(time_chart, 32, sizeof(char) * CHAR_ARRAYMAX); //check memset -> 32 -> space
	struct process* jobs_queue = malloc(sizeof(struct process) * NUM_PROCESS); //initialize arrays with size of NUM_PROCESS
	memcpy(jobs_queue, jobs_list, (sizeof(struct process) * NUM_PROCESS)); //copy processes into newArrivalArray
	SortBy(&jobs_queue, 0); //sort by arrival time
	PrintProcessList(jobs_queue); //print list of processes
	//bool* finished = malloc(sizeof(bool) * NUM_PROCESS);
	//memset(finished, FALSE, sizeof(bool) * NUM_PROCESS);
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
		while(curr_quantum < jobs_queue[first_unexecuted_job_idx].arrival_time) {
			//curr_quantum++; //increment curr quantum
			//time_chart[time_chart_idx++] = '-'; //update time chart
			time_chart[curr_quantum++] = '-'; //update time chart
		}
		//curr_quantum >= queue[0].arrival -> at least one process arrived and not executed yet
		//find out the shortest one among all arrived jobs that not finished yet
		int shortest_idx = first_unexecuted_job_idx; //0;
		float shortest_time = SERVICE_TIME_UPPER_BOUND;
		for(int i = first_unexecuted_job_idx; i < NUM_PROCESS; i++) { //i = 0
			if(jobs_queue[i].arrival_time <= curr_quantum && !finished[i] && jobs_queue[i].service_time < shortest_time) { //a process with shorter service time found
				shortest_idx = i; //update idx
				shortest_time = jobs_queue[i].service_time;
			}
		}
		//shortest job -> the job to run found -> set the actual start time
		jobs_queue[shortest_idx].actual_start_time = curr_quantum;
		float actual_end_time = curr_quantum + jobs_queue[shortest_idx].service_time; //might not be an integer
		char curr_job_name = jobs_queue[shortest_idx].name;
		while (curr_quantum < actual_end_time) {
			//curr_quantum++; //increment quantum
			//time_chart[time_chart_idx++] = curr_job_name; //jobs_queue[shortest_idx].name; //print curr job name to record -> time chart
			time_chart[curr_quantum++] = curr_job_name; //print curr job name to record -> time chart
		}
		//curr_quantum >= actual_end_time now
		//print curr job
		//PrintProcess(jobs_queue[shortest_idx]);
		jobs_queue[shortest_idx].end_time = curr_quantum;
		finished[shortest_idx] = true; //update finished
		num_finished_jobs++; //increment num of finished jobs
		if(curr_quantum >= MAX_QUANTA - 1 || num_finished_jobs == NUM_PROCESS) {
			done = true; //max quanta reached or no more job to run
		}
	}
	for (int i = 0; i < CHAR_ARRAYMAX; i++) {
		printf("%c", time_chart[i]);
	}
	//printf("\n");

	printf("\nAverage turnaround time: %.2f\n", AverageTAT(jobs_queue, NUM_PROCESS));
	printf("Average response time: %.2f\n", AverageRT(jobs_queue, NUM_PROCESS));
	printf("Average waiting time: %.2f\n", AverageWT(jobs_queue, NUM_PROCESS));
	printf("Throughput in 100 quanta: %.d\n", num_finished_jobs);

	free(jobs_queue); //don't forget to free the memory space for malloc
	
	return time_chart; //why return it?
}
