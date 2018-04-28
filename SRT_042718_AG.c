#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include "SRT.h"
#include <stdbool.h>
#include "Calculation.h"

// while processing job, iniend_time will be increment
void update_initend_time(struct process* candidates[], int num) {
   for (int i = 1; i < num; ++i) {
      candidates[i]->initend_time++;
   }
}

//sort algo for dynamic size
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

// romove first job in candidate queue
void remove_first(struct process* number[], int* num) {
    for (int i = 1; i < *num; ++i) {
        number[i - 1] = number[i];
    }
    (*num)--;
}

void SRT(struct process* plist)
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
    // the number of finish job
    int finishJob = 0;

    // the time chart
    char *chart = malloc(sizeof(char) * CHAR_ARRAYMAX);


    while (true) {
        // there is a job running now
        if (num > 0) {
          chart[time_index] = (num == 0)? '-' : candidates[0]->name;
          // modify candidates initend_time
          update_initend_time(candidates, num);

          // current process finish at this time
          if ((int)ceil(candidates[0]->initend_time) <= time_index) {
        //printf("JOB %c finish at %i\n", candidates[0]->name, time_index);
              
              // set current end time
              candidates[0]->end_time = time_index;
              // remove current from candidates
              candidates[0] = candidates[num - 1];
              num--;
              //count for finished job
              finishJob ++;
              
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

      if (num > 0) {
          // sort the candidates by end time
          quicksort(candidates, 0, num - 1);
          
          //set start time
          while(candidates[0]->actual_start_time == -1 && num > 0){
              if (time_index >= MAX_QUANTA) {
                  remove_first(candidates, &num);
                  continue;
              }
              candidates[0]->actual_start_time = time_index;
              break;
          }
          if (num > 0) {
              chart[time_index] = candidates[0]->name;
          }
          
      }
      time_index++;
   }
    PrintProcessListPre(copy_list);
    printCharTable(chart);
    printf("Average response time: %.2f\n", AverageRT(copy_list, NUM_PROCESS));
    printf("Average waiting time: %.2f\n", AverageWTPre(copy_list, NUM_PROCESS));
    printf("Average turnaround time: %.2f\n", AverageTATPre(copy_list, NUM_PROCESS));
    printf("Throughput in 100 quanta: %d\n", calThroughput(chart, sizeof(chart)/sizeof(char)));
    free(chart);
}

