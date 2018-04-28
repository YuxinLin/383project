#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_producer.h"
#include "RR.h"
#include "HPFA.h"
#include "FCFS.h"

void RoundRobin(struct process* o_plist){
    int counter_char_array = 0;
    int process_completed = 0;
    int counter_queue_end = 1;
    int index_queue = 0;
    
    int quanta = 0;
    int head = 0;
    
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    float total_response_time = 0;
    
    char* arrayRROrder = malloc(sizeof(char) * CHAR_ARRAYMAX);
    memset(arrayRROrder, 32, sizeof(char) * CHAR_ARRAYMAX);
    
    struct process* plist = malloc(sizeof(struct process)* NUM_PROCESS);
    memcpy(plist, o_plist, sizeof(struct process) * NUM_PROCESS);
    
    struct process* queue = malloc(sizeof(struct process)* NUM_PROCESS);
    //struct process* runqueue = malloc(sizeof(struct process)* NUM_PROCESS);
    
    mergeSort(&plist, 0, NUM_PROCESS - 1);
    
    quanta = plist[0].arrival_time;
    
    if(counter_char_array < quanta){
        arrayRROrder[counter_char_array] = '-';
        counter_char_array++;
    }
    
    queue[0] = plist[0];
    queue[0].actual_start_time = 1;
    head++;
    printf(" PID | Arrival | Start | Priority | Service | Response |  End  | Turnaround\n");
    
    
    while(counter_queue_end > 0){
        if(quanta < 100){
            quanta++; //update quanta
            
            arrayRROrder[counter_char_array] = queue[index_queue].name;
            counter_char_array++;
            
            
            for(int i = head; i < NUM_PROCESS; i++){
                if(plist[i].arrival_time < quanta){
                    queue[counter_queue_end] = plist[i];
                    counter_queue_end++;
                }
                else{
                    head = i;
                    break;
                }
            }
            
            
            if(queue[index_queue].actual_start_time == -1){
                queue[index_queue].actual_start_time = quanta - 1;
            }
            queue[index_queue].service_time--;
            
            if(queue[index_queue].service_time <= 0){
                queue[index_queue].end_time = quanta + 1;
                
                process_completed += 1;

                total_waiting_time += queue[index_queue].end_time - queue[index_queue].arrival_time - queue[index_queue].service_time;
                total_turnaround_time += queue[index_queue].end_time - queue[index_queue].arrival_time;
                total_response_time += queue[index_queue].actual_start_time - queue[index_queue].arrival_time;
                /*
                printf(" %3c | %7.1f | %5d | %8d | %7.1f | %8.1f | %5.1d | %7.1f \n",
                       queue[index_queue].name, queue[index_queue].arrival_time, queue[index_queue].actual_start_time, queue[index_queue].priority,
                       queue[index_queue].service_time, queue[index_queue].actual_start_time - queue[index_queue].arrival_time,
                       queue[index_queue].end_time,
                       queue[index_queue].end_time - queue[index_queue].arrival_time);
                */
                counter_queue_end += -1;
                for(int i = index_queue; i < counter_queue_end; i++){
                    queue[i] = queue[i + 1];
                }
            }
            
            index_queue++;
            if(index_queue == counter_queue_end){
                index_queue = 0;
            }
        }
        else{
            quanta++; //update quanta
            
            for(int i = index_queue; i < counter_queue_end; i++){
                if(queue[i].actual_start_time == -1){
                    counter_queue_end = i;
                    break;
                }
            }

            arrayRROrder[counter_char_array] = queue[index_queue].name;
            counter_char_array++;
            
            queue[index_queue].service_time--;
            if(queue[index_queue].service_time <= 0){
                queue[index_queue].end_time = quanta + 1;

                process_completed += 1;

                total_waiting_time += queue[index_queue].end_time - queue[index_queue].arrival_time - queue[index_queue].service_time;
                total_turnaround_time += queue[index_queue].end_time - queue[index_queue].arrival_time;
                total_response_time += queue[index_queue].actual_start_time - queue[index_queue].arrival_time;
                /*
                printf(" %3c | %7.1f | %5d | %8d | %7.1f | %8.1f | %5.1d | %7.1f \n",
                       queue[index_queue].name, queue[index_queue].arrival_time, queue[index_queue].actual_start_time, queue[index_queue].priority,
                       queue[index_queue].service_time, queue[index_queue].actual_start_time - queue[index_queue].arrival_time,
                       queue[index_queue].end_time,
                       queue[index_queue].end_time - queue[index_queue].arrival_time);
                */
                counter_queue_end += -1;
                for(int i = index_queue; i < counter_queue_end; i++){
                    queue[i] = queue[i + 1];
                }
            }
            index_queue++;
            if(index_queue == counter_queue_end){
                index_queue = 0;
            }
            else if(index_queue > counter_queue_end){
                break;
            }
        
        }
    }
    printCharTable(arrayRROrder);

    printf("\n");

    printf("Average turnaround time: %.2f\n", total_turnaround_time/process_completed);
    printf("Average response time: %.2f\n", total_response_time/process_completed);
    printf("Average waiting time: %.2f\n", total_waiting_time/process_completed);
    printf("Throughput in 100 quanta: %.2f\n", 100.0*process_completed/quanta);
    printf("%d", quanta);

    free(queue); //don't forget to free the memory space for malloc
    free(plist);
}


