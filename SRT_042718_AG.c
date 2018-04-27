#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include "SRT.h"
#include <stdbool.h>

void update_initend_time(struct process* candidates[], int num) {
   for (int i = 1; i < num; ++i) {
      candidates[i]->initend_time++;
   }
}

void SortByAG(struct process* plist [], int num){
	int pivot_location = num - 1;
	int head = 0;
	int inc_dec = 1;
	float compare_num;

        compare_num = (*plist[pivot_location]).initend_time;
	while(true){
		if((*plist[head]).initend_time > compare_num && head < pivot_location){
			swap(&(*plist[head]), &(*plist[pivot_location]), sizeof((*plist[head])), sizeof((*plist[pivot_location])));
			swap(&pivot_location, &head, sizeof(pivot_location), sizeof(head));
			inc_dec *= -1;
			head += inc_dec;
		}
		else if((*plist[head]).initend_time < compare_num && head > pivot_location){
			swap(&(*plist[head]), &(*plist[pivot_location]), sizeof((*plist[head])), sizeof((*plist[pivot_location])));
			swap(&pivot_location, &head, sizeof(pivot_location), sizeof(head));
			inc_dec *= -1;
			head += inc_dec;
		}
		else head += inc_dec;
		if(head == pivot_location) break;

	}

	if(pivot_location > 0) SortByRecAG(plist, num, 0, pivot_location);
	if(pivot_location < num - 1) SortByRecAG(plist, num, pivot_location + 1, num - 1);
	
	return;
}

void SortByRecAG(struct process* plist[], int num, int range_lo, int range_hi){
	if(range_hi - range_lo < 1) return;
	int pivot_location = range_hi;
	int head = range_lo;
	int inc_dec = 1;
	float compare_num;

	compare_num = (*plist[pivot_location]).initend_time;
	while(true){
		if((*plist[head]).initend_time > compare_num && head < pivot_location){
			swap(&(*plist[head]), &(*plist[pivot_location]), sizeof((*plist[head])), sizeof((*plist[pivot_location])));
			swap(&pivot_location, &head, sizeof(pivot_location), sizeof(head));
			inc_dec *= -1;
			head += inc_dec;
		}
		else if((*plist[head]).initend_time < compare_num && head > pivot_location){
			swap(&(*plist[head]), &(*plist[pivot_location]), sizeof((*plist[head])), sizeof((*plist[pivot_location])));
			swap(&pivot_location, &head, sizeof(pivot_location), sizeof(head));
			inc_dec *= -1;
			head += inc_dec;
		}
		else head += inc_dec;
		if(head == pivot_location) break;
	}	
	if(pivot_location > 0) SortByRecAG(plist, num, range_lo, pivot_location - 1);
	if(pivot_location < num - 1) SortByRecAG(plist, num, pivot_location, range_hi);

	return;
}

void quicksort(struct process* number[], int first,int last){
   int i, j, pivot;
   struct process* temp;
   if(first<last){
      pivot=first;
      i=first;
      j=last;
      while(i<j){
         while(number[i]->initend_time<=number[pivot]->initend_time&&i<last)
            i++;
         while(number[j]->initend_time>number[pivot]->initend_time)
            j--;
         if(i<j){
            temp=number[i];
            number[i]=number[j];
            number[j]=temp;
         }
      }
      temp=number[pivot];
      number[pivot]=number[j];
      number[j]=temp;
      quicksort(number,first,j-1);
      quicksort(number,j+1,last);
   }
}


char* SRT(struct process* plist) 
{
   // make a copy of the process list, and sort by arrive time
   struct process* copy_list = malloc(sizeof(struct process) * NUM_PROCESS);
   memcpy(copy_list, plist, sizeof(struct process) * NUM_PROCESS);
   SortBy(&copy_list, 0); //The input num decides what field to sort by 0 - arrival time; 1 - service time; 2 - priority; 3 - actual start; 4 - end time


   // the representation of time
   int time_index = 0;
   // next process index
   int next = 0;
   // the current processing candidate process (index)
   struct process* candidates [NUM_PROCESS] = { NULL };
   // the number of candidates
   int num = 0;

   // the time chart
   char *chart = malloc(sizeof(char) * CHAR_ARRAYMAX);


   while (true) {

      if (num > 0) {
          // 1. there is a job running now
          
          // modify candidates initend_time
          update_initend_time(candidates, num);

          // 1.1.1 current process finish at this time
          if ((int)ceil(candidates[0]->initend_time) <= time_index) {

             printf("JOB %c finish at %i\n", candidates[0]->name, time_index);

             // set current end time
             candidates[0]->end_time = time_index;
             // remove current from candidates
             candidates[0] = candidates[num - 1];
             num--;
          } else {
             printf("working on JOB %c at %i initend on %i\n", candidates[0]->name, time_index, candidates[0]->initend_time);
          }

      }

      if (time_index >= MAX_QUANTA && num == 0) {
         // we finish \(0_0)/
         break;
      } 



      // add all process arrived now to candidates
      while ((int)ceil(copy_list[next].arrival_time) == time_index) {
         candidates[num++] = &copy_list[next++];
      }

      if (num > 1) {
         // sort the candidates by end time
         quicksort(candidates, 0, num - 1);
      }

      chart[time_index] = (num == 0)? '-' : candidates[0]->name;
      time_index++;
   }
   return chart;
}


void printSRT(struct process* plist) {

	int process_ran;
	char* charArray = SRT(plist);

	// Adjust to add extra spaces to match printProcessTracker();
	// Char array doesn't have extra spaces to accommodate the difference
	for (int i = 0; i < CHAR_ARRAYMAX; i++) {
	    printf("%c", charArray[i]);
	}
	printf("\n");
	// Size is now the # of processes that completed its process
	//printf("Average response time: %.2f\n", calAverageResponse(plist, process_ran));
	//printf("Average waiting time: %.2f\n", calAverageWaiting(plist, process_ran));
	//printf("Average turnaround time: %.2f\n", calAverageTurnaround(plist, process_ran));
	//printf("Throughput: %d\n", process_ran);
	free(charArray);
}

