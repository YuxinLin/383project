#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"

/*start of revision
 *add new version of calculation for tat / wait time / response time
 */

//turnaround time for non-preemtive
float AverageTAT(struct process jobs_list[], int size) {
	float total_tat = 0.0;
	int num_finished_process = 0;
	for(int i = 0; i < size; i++) {
		if(jobs_list[i].end_time > 0) { //[i] finished
			num_finished_process++;
			total_tat += (jobs_list[i].actual_start_time + jobs_list[i].service_time - jobs_list[i].arrival_time);
			//total_tat += (jobs_list[i].end_time - jobs_list[i].arrival_time);
		}
	}
	return num_finished_process > 0 ? total_tat / num_finished_process : 0;
}

//tat for preemtive
float AverageTATPre(struct process jobs_list[], int size) {
    float total_tat = 0.0;
    int num_finished_process = 0;
    for(int i = 0; i < size; i++) {
        if(jobs_list[i].end_time > 0) { //[i] finished
            num_finished_process++;
            //total_tat += (jobs_list[i].actual_start_time + jobs_list[i].service_time - jobs_list[i].arrival_time);
            total_tat += (jobs_list[i].end_time - jobs_list[i].arrival_time);
        }
    }
    return num_finished_process > 0 ? total_tat / num_finished_process : 0;
}

//waiting time for non-preemtive
float AverageWT(struct process jobs_list[], int size) {
	float total_wait_time = 0.0;
	int num_finished_process = 0;
	for(int i = 0; i < size; i++) {
		if(jobs_list[i].end_time > 0) { //[i] finished
			num_finished_process++;
			total_wait_time += (jobs_list[i].actual_start_time - jobs_list[i].arrival_time);
			//total_wait_time += (jobs_list[i].actual_start_time + jobs_list[i].service_time - jobs_list[i].arrival_time - jobs_list[i].service_time);
			//total_wait_time += (jobs_list[i].end_time - jobs_list[i].arrival_time - jobs_list[i].service_time);
		}
	}
	return num_finished_process > 0 ? total_wait_time / num_finished_process : 0;
}

//waiting time for preemtive
float AverageWTPre(struct process jobs_list[], int size) {
    float total_wait_time = 0.0;
    int num_finished_process = 0;
    for(int i = 0; i < size; i++) {
        if(jobs_list[i].end_time > 0) { //[i] finished
            num_finished_process++;
            //total_wait_time += (jobs_list[i].actual_start_time - jobs_list[i].arrival_time);
            //total_wait_time += (jobs_list[i].actual_start_time + jobs_list[i].service_time - jobs_list[i].arrival_time - jobs_list[i].service_time);
            total_wait_time += (jobs_list[i].end_time - jobs_list[i].arrival_time - jobs_list[i].service_time);
        }
    }
    return num_finished_process > 0 ? total_wait_time / num_finished_process : 0;
}

//response time
float AverageRT(struct process jobs_list[], int size) {
	float total_response_time = 0.0;
	int num_finished_process = 0;
	for(int i = 0; i < size; i++) {
		if(jobs_list[i].end_time > 0) { //[i] finished
			num_finished_process++;
			total_response_time += (jobs_list[i].actual_start_time - jobs_list[i].arrival_time);
		}
	}
	return num_finished_process > 0 ? total_response_time / num_finished_process : 0;
}

/*
 *end of revision by Peng
 */

float calAverageTurnaround(struct process plist[], int size) {
	float turnaroundTime = 0.0;
	int incomplete_process = 0;
	for (int i = 0; i < size; i++) {
		// Turn around time is the time from the arrival to end of a process
		if(plist[i].end_time > plist[i].arrival_time){
		turnaroundTime += plist[i].end_time - plist[i].arrival_time;
		}
		else incomplete_process++;
	}
	return (turnaroundTime / (size - incomplete_process));
}

float calAverageWaiting(struct process plist[], int size) {
	float avgWaiting = 0.0;

	for (int i = 0; i < size; i++) {
		// Waiting time is the time to wait to start
		avgWaiting += plist[i].actual_start_time - plist[i].arrival_time;//adjust to throw error if actual is lower than arrival?
	}

	return (avgWaiting / size);
}

float calAverageResponse(struct process plist[], int size) {
	float responseTime = 0.0;
	int incomplete_process = 0;
	for (int i = 0; i < size; i++) {
		// Response time is the actual starting time to when the first output is produced
		if(plist[i].actual_start_time >= plist[i].arrival_time){
		responseTime += plist[i].actual_start_time - plist[i].arrival_time;
		}
		else incomplete_process++;
	}
	return (responseTime / (size - incomplete_process));
}



float calAverageResponse2(struct process plist[], int size) {
	float responseTime = 0.0;

	for (int i = 0; i < size; i++) {
		// Response time is the actual starting time to when the first output is produced
		responseTime += plist[i].end_time - plist[i].actual_start_time;
	}
	return (responseTime / size);
}

int calThroughput(char charArray[], int size) {
	// Only processes that are completed before the [0-99] quanta range are counted.
	int throughput = size;
	
	if (charArray[100] != ' ') {
		// One process was still running after [0-99] quanta
		throughput = size - 1;
	} else {
		throughput = size;
	}

	return throughput;
}

