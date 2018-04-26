#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"

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

