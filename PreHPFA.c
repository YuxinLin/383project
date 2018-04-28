#include <stdio.h>
#include <stdlib.h>
#include "Sorter.h"
#include "Calculation.h"
#include "HPFA.h"
#include "PreHPFA.h"
#define TRUE 1 //add header to store define variables

void PreHPF(struct process* plist){
	int numberOfProcessCompleted = 0;
	char* arrayHPFOrder = malloc(sizeof(char) * 300); //Allocate enough to leave spaces for last process to finish
	memset(arrayHPFOrder, 32, sizeof(char) * 300);
	int quanta = 0;

	/**
	Initializing the sizes,indexes and 4 priority queues. 1 is highest priority->4 lowest
	service_time is used to reupdate the service times after all the processing, which is then
	used for calculation of statistics.
	**/
	int size1 = findSize(plist,1); int index1 = 0; int runningIndex1=index1;
	int size2 = findSize(plist,2); int index2 = 0; int runningIndex2=index2;
	int size3 = findSize(plist,3); int index3 = 0; int runningIndex3=index3;
	int size4 = findSize(plist,4); int index4 = 0; int runningIndex4=index4;
	struct process* queue1 = malloc(sizeof(struct process)*size1);
	struct process* queue2 = malloc(sizeof(struct process)*size2);
	struct process* queue3 = malloc(sizeof(struct process)*size3);
	struct process* queue4 = malloc(sizeof(struct process)*size4);
	float* service_time1 = malloc(sizeof(float)*size1);
	float* service_time2 = malloc(sizeof(float)*size2);
	float* service_time3 = malloc(sizeof(float)*size3);
	float* service_time4 = malloc(sizeof(float)*size4);
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
	/*
	printf("queue1: \n");
	for(int i=0;i<size1;i++){
		printf(" %c ",queue1[i].name);
	}
	*/

	//Saving the service times for each so that i can update them later to calulate the statistics

	for(int i=0;i<size1;i++){
		service_time1[i]=queue1[i].service_time;
	}
	for(int i=0;i<size2;i++){
		service_time2[i]=queue2[i].service_time;
	}	
	for(int i=0;i<size3;i++){
		service_time3[i]=queue3[i].service_time;
	}	
	for(int i=0;i<size4;i++){
		service_time4[i]=queue4[i].service_time;
	}

/**
Split between 4 "priority" queues for processing
**/
	for(int i=0; i<NUM_PROCESS && quanta<MAX_QUANTA;i++){
		/**
		Index# 
			- Smallest arrival time index in this queue that I have not completed yet. I need to maintain this value
			so that I can loop back for round robin processing
		RunningIndex#
			- Starts from the index#, and goes one by one to the next valid values of the queue. By valid, I mean values
			where the arrival_time<=quanta, and service_time>0
		**/
		if(index1<size1 && queue1[index1].arrival_time<=quanta){
			while(queue1[runningIndex1].service_time>0 && queue1[runningIndex1].arrival_time<=quanta){
				if(queue1[runningIndex1].actual_start_time<=0){
					queue1[runningIndex1].actual_start_time=quanta;
				}
				queue1[runningIndex1].service_time-=1;
				if(queue1[runningIndex1].service_time<=0){
					queue1[runningIndex1].end_time=quanta+1;
					numberOfProcessCompleted++;
					if(runningIndex1==index1){
						while(index1<size1 && queue1[index1].service_time<=0){
							index1++;
						}
					}
				}
				arrayHPFOrder[quanta++] = queue1[runningIndex1].name;
				quanta1++;
				if(index1==size1){
					runningIndex1=index1;
					break;
				}
				/**
				In this section, if you reach the end of the queue, you need to start at the index again. 
				Or, if your next value is > quanta, then you need to go back to the index. 
				If not, then increment the next pointer, since it means that your next value will be both 
				not out of the queue, and also it's arrival_time<=quanta. However, if the new runningIndex# 
				is a process where you already finished processing, then you need to find a new process, which
				you need to check is not an edge or out of the queue, hence the last while loop in this queue.
				**/
				if(runningIndex1==size1-1 || (queue1[runningIndex1+1].arrival_time > quanta)){
					runningIndex1=index1;
				}
				else if(queue1[runningIndex1+1].arrival_time <= quanta){
					runningIndex1++;
					while(queue1[runningIndex1].service_time<=0){
						if(runningIndex1==size1-1 || (queue1[runningIndex1+1].arrival_time > quanta)){
							runningIndex1=index1;
							break;
						}
						runningIndex1++;
					}
				}
			}
		}

		/**
		All the other queues are basically the same implementation, however, there is one difference. The first while
		loop contains the statement where (index1<size1 && queue1[index1].arrival_time>quanta) This statement is what
		makes this priority queue implementation "preemptive". Once it sees that the "index", "smallest arrival time"
		of a higher priority queue has become <=quanta, it needs to exit this queue and go process the process from the 
		higher priority queue, which in this case is queue1. Note queue3 contains "interrupts" for both queue2 and queue1
		...etc 
		**/
		else if(index2<size2 && queue2[index2].arrival_time<=quanta){
			while((index1<size1 && queue1[index1].arrival_time>quanta) && queue2[runningIndex2].service_time>0 && 
				queue2[runningIndex2].arrival_time<=quanta){

				if(queue2[runningIndex2].actual_start_time<=0){
					queue2[runningIndex2].actual_start_time=quanta;
				}
				queue2[runningIndex2].service_time-=1;
				if(queue2[runningIndex2].service_time<=0){
					queue2[runningIndex2].end_time=quanta+1;
					numberOfProcessCompleted++;
					while(index2<size2 && queue2[index2].service_time<=0){
						index2++;
					}
				}
				arrayHPFOrder[quanta++] = queue2[runningIndex2].name;
				quanta2++;
				if(index2==size2){
					runningIndex2=index2;
					break;
				}
				
				if(runningIndex2+1==size2 || (queue2[runningIndex2+1].arrival_time > quanta)){
					runningIndex2=index2;
				}
				else if(queue2[runningIndex2+1].arrival_time <= quanta){
					runningIndex2++;
					while(queue2[runningIndex2].service_time<=0){
						if(runningIndex2==size2-1 || (queue2[runningIndex2+1].arrival_time > quanta)){
							runningIndex2=index2;
							break;
						}
						runningIndex2++;
					}
				}
				
			}
			
		}
		else if(index3<size3 && queue3[index3].arrival_time<=quanta){
			
			while((index1<size1 && queue1[index1].arrival_time>quanta) && (index2<size2 && queue2[index2].arrival_time>quanta) 
				&& queue3[runningIndex3].service_time>0 && queue3[runningIndex3].arrival_time<=quanta){

				if(queue3[runningIndex3].actual_start_time<=0){
					queue3[runningIndex3].actual_start_time=quanta;
				}
				queue3[runningIndex3].service_time-=1;
				if(queue3[runningIndex3].service_time<=0){
					queue3[runningIndex3].end_time=quanta+1;
					numberOfProcessCompleted++;				
					while(index3<size3 && queue3[index3].service_time<=0){
						index3++;
					}
				}
				arrayHPFOrder[quanta++] = queue3[runningIndex3].name;
				quanta3++;

				if(index3==size3){
					runningIndex3=index3;
					break;
				}

				if(runningIndex3+1==size3 || (queue3[runningIndex3+1].arrival_time > quanta)){
					runningIndex3=index3;
				}
				else if(queue3[runningIndex3+1].arrival_time <= quanta){
					runningIndex3++;
					while(queue3[runningIndex3].service_time<=0){
						if(runningIndex3==size3-1 || (queue3[runningIndex3+1].arrival_time > quanta)){
							runningIndex3=index3;
							break;
						}
						runningIndex3++;
					}
				}
			}
		}
		else if(index4<size4 && queue4[index4].arrival_time<=quanta){
			while((index1<size1 && queue1[index1].arrival_time>quanta) && (index2<size2 && queue2[index2].arrival_time>quanta) 
				&& (index3<size3 && queue3[index3].arrival_time>quanta) && queue4[runningIndex4].service_time>0 && 
				queue4[runningIndex4].arrival_time<=quanta){

				if(queue4[runningIndex4].actual_start_time<=0){
					queue4[runningIndex4].actual_start_time=quanta;
				}
				queue4[runningIndex4].service_time-=1;
				if(queue4[runningIndex4].service_time<=0){
					queue4[runningIndex4].end_time=quanta+1;
					numberOfProcessCompleted++;
					while(index4<size4 && queue4[index4].service_time<=0){
					index4++;
					}
				}
				arrayHPFOrder[quanta++] = queue4[runningIndex4].name;
				quanta4++;
				if(index4==size4){
					runningIndex4=index4;
					break;
				}
				
				if(runningIndex4+1==size4 || (queue4[runningIndex4+1].arrival_time > quanta)){
					runningIndex4=index4;
				}
				else if(queue4[runningIndex4+1].arrival_time <= quanta){
					runningIndex4++;
					while(queue4[runningIndex4].service_time<=0){
						if(runningIndex4==size4-1 || (queue4[runningIndex4+1].arrival_time > quanta)){
							runningIndex4=index4;
							break;
						}
						runningIndex4++;
					}
				}
			}
		}
		else{
			arrayHPFOrder[quanta++]='-';
		}
	}

	/**
	Reupdating the values of the service_time of each process in the queue, since it needs to calculate statistics
	**/
	int completed1=0; int completed2=0; int completed3=0; int completed4=0;
	for(int i=0;i<size1;i++){
		if(queue1[i].service_time<=0){
			completed1++;
		}
		queue1[i].service_time = service_time1[i];
	}
	for(int i=0;i<size2;i++){
		if(queue2[i].service_time<=0){
			completed2++;
		}
		queue2[i].service_time = service_time2[i];
	}	
	for(int i=0;i<size3;i++){
		if(queue3[i].service_time<=0){
			completed3++;
		}
		queue3[i].service_time = service_time3[i];
	}	
	for(int i=0;i<size4;i++){
		if(queue4[i].service_time<=0){
			completed4++;
		}
		queue4[i].service_time = service_time4[i];
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
		printf("%c ", arrayHPFOrder[i]);
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
	Printing values
	**/
	printf("Priority Queue 1 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (completed1==0)?0 : AverageTATPre(queue1,size1));
	printf("Average waiting time:\t\t%.2f\n", (completed1==0)?0 : AverageWTPre(queue1, size1));
	printf("Average response time:\t\t%.2f\n", (completed1==0)?0 : AverageRT(queue1, size1));

	//printf("index1: %d  quanta1: %d\n", index1, quanta1);
	printf("Throughput: %.2f\n", (completed1==0 || quanta1==0)?0 : 100.0*completed1/quanta1);
	printf("\n");

	printf("Priority Queue 2 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (completed2==0)?0 : AverageTATPre(queue2,size2));
	printf("Average waiting time:\t\t%.2f\n", (completed2==0)?0 : AverageWTPre(queue2,size2));
	printf("Average response time:\t\t%.2f\n", (completed2==0)?0 : AverageRT(queue2,size2));
	//printf("index2: %d  quanta2: %d\n", index2, quanta2);
	printf("Throughput: %.2f\n", (completed2==0 || quanta2==0)?0 : 100.0*completed2/quanta2);

	printf("\n");
	printf("Priority Queue 3 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (completed3==0)?0 : AverageTATPre(queue3,size3));
	printf("Average waiting time:\t\t%.2f\n", (completed3==0)?0 : AverageWTPre(queue3, size3));
	printf("Average response time:\t\t%.2f\n", (completed3==0)?0 : AverageRT(queue3, size3));
	//printf("index3: %d  quanta3: %d\n", index3, quanta3);
	printf("Throughput: %.2f\n", (completed3==0 || quanta3==0)?0 : 100.0*completed3/quanta3);

	printf("\n");
	printf("Priority Queue 4 Averages:\n");
	printf("Average turnaround time:\t%.2f\n", (completed4==0)?0 : AverageTATPre(queue4,size4));
	printf("Average waiting time:\t\t%.2f\n", (completed4==0)?0 : AverageWTPre(queue4, size4));
	printf("Average response time:\t\t%.2f\n", (completed4==0)?0 : AverageRT(queue4, size4));
	//printf("index4: %d  quanta4: %d\n", index4, quanta4);
	printf("Throughput: %.2f\n", (completed4==0 || quanta4==0)?0 : 100.0*completed4/quanta4);


	printf("\n");
	printf("Overall Averages:\n");
	printf("Average turnaround time:\t%.2f\n", AverageTATPre(plist,NUM_PROCESS));
	printf("Average waiting time:\t\t%.2f\n", AverageWTPre(plist,NUM_PROCESS));
	printf("Average response time:\t\t%.2f\n", AverageRT(plist,NUM_PROCESS));
	printf("Throughput in 100 quanta: %.2f\n", 100.0*numberOfProcessCompleted/quanta);
}