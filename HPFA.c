#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_producer.h"
#include "Calculation.h"
#include "Sorter.h"
#include "HPFA.h"
#define TRUE 1
#define FALSE 0

/**
Used to find size of each priority queue. 
**/
int findSize(struct process* plist, int priority){
	int count=0;
	for(int i=0;i<NUM_PROCESS;i++){
		if(plist[i].priority<priority){
			continue;
		}
		else if(plist[i].priority>priority){
			break;
		}
		else{
			count++;
		}

	}
	return count;
}

/**
Merge sort for priority queues
**/
void merge(struct process** queue, int left, int mid, int right){
	int sizeLeft = mid-left+1;
	int sizeRight = right-mid;
	struct process* leftQueue = malloc(sizeof(struct process)*sizeLeft);
	struct process* rightQueue = malloc(sizeof(struct process)*sizeRight);

	for(int i=0;i<sizeLeft;i++){
		leftQueue[i] = (*queue)[left+i];
	}
	for(int i=0;i<sizeRight;i++){
		rightQueue[i] = (*queue)[mid+1+i];
	}

	int i=0; int j=0; int k=left;

	while(i<sizeLeft && j<sizeRight){
		if(leftQueue[i].arrival_time <= rightQueue[j].arrival_time){
			(*queue)[k] = leftQueue[i];
			i++;
		}
		else{
			(*queue)[k] = rightQueue[j];
			j++;
		}
		k++;
	}

	while(i<sizeLeft){
		(*queue)[k] = leftQueue[i];
		i++; k++;
	}
	while(j<sizeRight){
		(*queue)[k] = rightQueue[j];
		j++; k++;
	}
}

void mergeSort(struct process** queue, int left, int right){
	if(left<right){
		int mid = (left+right)/2;
		mergeSort(queue, left, mid);
		mergeSort(queue, mid+1, right);
		merge(queue, left, mid, right);
	}
}
/*
float AverageTAT(struct process* list, int size){
	float total_tat = 0.0;
	int num_finished_process=0;
	for(int i=0;i<size;i++){
		if(list[i].end_time>0){
			num_finished_process++;
			total_tat += (list[i].actual_start_time+list[i].service_time-list[i].arrival_time);
		}
	}
	return total_tat/num_finished_process;
}

float AverageWT(struct process* list, int size){
	float total_wait_time = 0.0;
	int num_finished_process=0;
	for(int i=0;i<size;i++){
		if(list[i].end_time>0){
			num_finished_process++;
			total_wait_time += (list[i].actual_start_time-list[i].arrival_time);
		}
	}
	return total_wait_time/num_finished_process;
}

float AverageRT(struct process* list, int size){
	float total_response_time =0.0;
	int num_finished_process=0;
	for(int i=0;i<size;i++){
		if(list[i].end_time>0){
			num_finished_process++;
			total_response_time += (list[i].actual_start_time-list[i].arrival_time);
		}
	}
	return total_response_time/num_finished_process;
}
*/

void nonpreHPF(struct process* plist){
	int numberOfProcessCompleted = 0;
	char* arrayHPFOrder = malloc(sizeof(char) * CHAR_ARRAYMAX); //Allocate enough to leave spaces for last process to finish
	memset(arrayHPFOrder, 32, sizeof(char) * CHAR_ARRAYMAX);
	int quanta = 0;
	/**
	Initializing the sizes,indexes and 4 priority queues. 1 is highest priority->4 lowest
	**/
	int size1 = findSize(plist,1); int index1 = 0;
	int size2 = findSize(plist,2); int index2 = 0;
	int size3 = findSize(plist,3); int index3 = 0;
	int size4 = findSize(plist,4); int index4 = 0;
	struct process* queue1 = malloc(sizeof(struct process)*size1);
	struct process* queue2 = malloc(sizeof(struct process)*size2);
	struct process* queue3 = malloc(sizeof(struct process)*size3);
	struct process* queue4 = malloc(sizeof(struct process)*size4);
	int quanta1=0; int quanta2=0; int quanta3=0; int quanta4=0;
	/**
	Since the plist is sorted in terms of priority, I used findSize above to find how many of each priorities there are.
	Then I put them in their own queues using offsets of size1 to size4
	**/
	for(int i=0;i<size1;i++){
		queue1[i]=plist[i];
	}
	int k=0;
	for(int i=size1;i<size1+size2;i++){
		queue2[k++]=plist[i];
	}
	k=0;
	for(int i=size1+size2;i<size1+size2+size3;i++){
		queue3[k++]=plist[i];
	}
	k=0;
	for(int i=size1+size2+size3;i<NUM_PROCESS;i++){
		queue4[k++]=plist[i];
	}

	/**
	Sorting the priority queues based on their arrival times now, so I have 4 priority queues, each sorted by arrival time
	**/
	mergeSort(&queue1,0,size1-1);
	mergeSort(&queue2,0,size2-1);
	mergeSort(&queue3,0,size3-1);
	mergeSort(&queue4,0,size4-1);


	/**
	Goes through queues in terms of arrival time. Checks the queue1 first, if it does not find a valid process, check the other queues
	in order of priority.

	Within each "if/elseif" it sets the actual_start_time of each process, processes it for the amount of "service time" it requires
	and then updates the end time and number of processes completed.
	**/
	for(int i=0;i<NUM_PROCESS && quanta<MAX_QUANTA;i++){
		if(index1<size1 && queue1[index1].arrival_time <= quanta){
			queue1[index1].actual_start_time = quanta;
			float temp_service_time = queue1[index1].service_time;
			while(temp_service_time>0){
				arrayHPFOrder[quanta++]=queue1[index1].name;
				temp_service_time-=1;
				quanta1++;
			}
			queue1[index1++].end_time=quanta;
			numberOfProcessCompleted++; 
		}
		else if(index2<size2 && queue2[index2].arrival_time <= quanta){
			queue2[index2].actual_start_time = quanta;
			float temp_service_time = queue2[index2].service_time;
			while(temp_service_time>0){
				arrayHPFOrder[quanta++]=queue2[index2].name;
				temp_service_time-=1;
				quanta2++;
			}
			queue2[index2++].end_time=quanta;
			numberOfProcessCompleted++; 
		}
		else if(index3<size3 && queue3[index3].arrival_time <= quanta){
			queue3[index3].actual_start_time = quanta;
			float temp_service_time = queue3[index3].service_time;
			while(temp_service_time>0){
				arrayHPFOrder[quanta++]=queue3[index3].name;
				temp_service_time-=1;
				quanta3++;
			}
			queue3[index3++].end_time=quanta;
			numberOfProcessCompleted++;
		}
		else if(index4<size4 && queue4[index4].arrival_time <= quanta){
			queue4[index4].actual_start_time = quanta;
			float temp_service_time = queue4[index4].service_time;
			while(temp_service_time>0){
				arrayHPFOrder[quanta++]=queue4[index4].name;
				temp_service_time-=1;
				quanta4++;
			}
			queue4[index4++].end_time=quanta;
			numberOfProcessCompleted++; 
		}
		else{
			arrayHPFOrder[quanta++]='-';
		}
	}

	printCharTable(arrayHPFOrder);
	//printf("FINAL QUANTA: %d\n", quanta);
	/**
	Printing the time chart of 100+ quantas. I have it set at 150 max entries in arrayHPFOrder, while maxquanta is 100
	**/
	/*
	printf("\nProcess Tracker (Hold control key and hit - key to zoom out in terminal to see)\n"); // need a more elegant solution than this
	for (int i = 0; i < quanta+1; i++) {
		printf("%d ", i);
	}
	printf("\n");

	printf("\n\n");
	for(int i=0;i<quanta+1;i++){
		printf("%c ",arrayHPFOrder[i]);
	}
	printf("\n\n");
	*/
	/**
	Puts back all the entries in the priority queues back into the plist, so that we can calculate the total statistics for all entries
	Ideally, I should have made the queues double pointer arrays to access the plist without needing to put it back, but too late. 
	**/
	int j=0;k=0;int l=0;int m=0;
	for(int i=0;i<NUM_PROCESS;i++){
		if(j<size1){
			plist[i]=queue1[j++];
		}
		else if(k<size2){
			plist[i]=queue2[k++];
		}
		else if(l<size3){
			plist[i]=queue3[l++];
		}
		else if(m<size4){
			plist[i]=queue4[m++];
		}
	}
	/**
	Printing valuesrvice Time: 4.3 Priority: 01 Arrival Time: 17.10
0: Service Time: 0.2 Priority: 02 Arrival Time: 0.80

	**/
	printf("Priority Queue 1 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (index1==0)?0 : AverageTAT(queue1,size1));
	printf("Average waiting time:\t\t%.2f\n", (index1==0)?0 : AverageWT(queue1, size1));
	printf("Average response time:\t\t%.2f\n", (index1==0)?0 : AverageRT(queue1, size1));
	//printf("index1: %d  quanta1: %d\n", index1, quanta1);
	printf("Throughput: %.2f\n", (index1==0 || quanta1==0)?0 : 100.0*index1/quanta1);
	printf("\n");

	printf("Priority Queue 2 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (index2==0)?0 : AverageTAT(queue2,size2));
	printf("Average waiting time:\t\t%.2f\n", (index2==0)?0 : AverageWT(queue2,size2));
	printf("Average response time:\t\t%.2f\n", (index2==0)?0 : AverageRT(queue2,size2));
	//printf("index2: %d  quanta2: %d\n", index2, quanta2);
	printf("Throughput: %.2f\n", (index2==0 || quanta2==0)?0 : 100.0*index2/quanta2);

	printf("\n");
	printf("Priority Queue 3 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (index3==0)?0 : AverageTAT(queue3,size3));
	printf("Average waiting time:\t\t%.2f\n", (index3==0)?0 : AverageWT(queue3, size3));
	printf("Average response time:\t\t%.2f\n", (index3==0)?0 : AverageRT(queue3, size3));
	//printf("index3: %d  quanta3: %d\n", index3, quanta3);
	printf("Throughput: %.2f\n", (index3==0 || quanta3==0)?0 : 100.0*index3/quanta3);

	printf("\n");
	printf("Priority Queue 4 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (index4==0)?0 : AverageTAT(queue4,size4));
	printf("Average waiting time:\t\t%.2f\n", (index4==0)?0 : AverageWT(queue4, size4));
	printf("Average response time:\t\t%.2f\n", (index4==0)?0 : AverageRT(queue4, size4));
	//printf("index4: %d  quanta4: %d\n", index4, quanta4);
	printf("Throughput: %.2f\n", (index4==0 || quanta4==0)?0 : 100.0*index4/quanta4);


	printf("\n");
	printf("Overall Averages:\n");
	printf("Average turnaround time:\t%.2f\n", AverageTAT(plist,NUM_PROCESS));
	printf("Average waiting time:\t\t%.2f\n", AverageWT(plist,NUM_PROCESS));
	printf("Average response time:\t\t%.2f\n", AverageRT(plist,NUM_PROCESS));
	printf("Throughput in 100 quanta: %.2f\n", 100.0*numberOfProcessCompleted/quanta);
}
