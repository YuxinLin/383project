#include <stdio.h>
#include <stdlib.h>
#include "process_producer.h"
#include "Calculation.h"
#include "ShortestJobFirst.h"
#include <string.h>
#include "Sorter.h"


char* shortestJobFirst(struct process* o_plist) //process list is an array with all the processes 
//with info about arrival time and service time. 

{
	int currentQuanta; //keep track of the quanta during odering.
	int size = 0;
    int charIndex = 0;

    char* arrayFCFSOrder = malloc(sizeof(char) * CHAR_ARRAYMAX); // Allocate enough to leave spaces for last process to finish 
    memset(arrayFCFSOrder, 32, sizeof(char) * CHAR_ARRAYMAX);
	struct process* newArrivalArray = malloc(sizeof(struct process) * NUM_PROCESS); //initialize arrays with size of NUM_PROCESS

	struct process* temp; //this is a single empty process called temp that I use for swapping.
    
    struct process* shortJobsExecuted = malloc(sizeof(struct process) * NUM_PROCESS); //create new array of shortsJobcompleted
   
   char CompletedProcesses [NUM_PROCESS]; //Just to put the completed processes in here for testing

	memcpy(newArrivalArray, o_plist, (sizeof(struct process) * NUM_PROCESS)); //copy processes into newArrivalArray
	
	SortBy(&newArrivalArray, 0); //Sort processes in array by arrival times only.

	newArrivalArray[0].actual_start_time = newArrivalArray[0].arrival_time; 
	size++;

	//Since the first process will always be started.

    for (int x = 0; x < newArrivalArray[0].arrival_time; x++) {
        arrayFCFSOrder[x] = '-';
        charIndex++;
    }

    for (int x = newArrivalArray[0].arrival_time; x <= newArrivalArray[0].service_time; x++) {
        arrayFCFSOrder[x] = newArrivalArray[0].name;
        charIndex++;
    }

	currentQuanta = newArrivalArray[0].arrival_time + newArrivalArray[0].service_time; 

    newArrivalArray[0].end_time = currentQuanta;

	for(int j=1; j<NUM_PROCESS; j++) //we start at 1 because the first process (0 index) will start first always
	{ //each index is a Process.
		
			//swap around indexes within serviceArray
			if(newArrivalArray[j+1].arrival_time <= currentQuanta && newArrivalArray[j+1].arrival_time <= currentQuanta) //If Both processes arrived already.
			{
					if(newArrivalArray[j+1].service_time < newArrivalArray[j].service_time) //compare service times
					{
                        for (int x = 0; x < newArrivalArray[j].service_time; x++) {
                            arrayFCFSOrder[charIndex] = newArrivalArray[j].name;
                            charIndex++;
                        }


						temp = &newArrivalArray[j]; //added address symbol infront of array
						newArrivalArray[j] = newArrivalArray[j+1];
						newArrivalArray[j+1] = *temp; //dereferencing
						newArrivalArray[j].actual_start_time = currentQuanta; //this is the actual start time for each process
						
                       
                        currentQuanta = currentQuanta + newArrivalArray[j].service_time;
						size++;
                        newArrivalArray[j].end_time = currentQuanta;
                        
                       
					}
					else
					{

                         for (int x = 0; x < newArrivalArray[j].service_time; x++) {
                            arrayFCFSOrder[charIndex] = newArrivalArray[j].name;
                            charIndex++;
                        }
                          
						newArrivalArray[j].actual_start_time = currentQuanta;
						currentQuanta = currentQuanta + newArrivalArray[j].service_time;
                        size++;
                        newArrivalArray[j].end_time = currentQuanta;
                        
					}

			}
			else if (newArrivalArray[j].arrival_time <= currentQuanta && newArrivalArray[j+1].arrival_time > currentQuanta)
			{

                
                for (int x = 0; x < newArrivalArray[j].service_time; x++) {
                    arrayFCFSOrder[charIndex] = newArrivalArray[j].name;
                    charIndex++;
                }

				//then we keep the process of j in that position since j+1 hasnt even arrived.
				newArrivalArray[j].actual_start_time = currentQuanta;

				currentQuanta = currentQuanta + newArrivalArray[j].service_time;
                newArrivalArray[j].end_time = currentQuanta;
                size++;
                
			}

			else //This whole condition is for when neither processes have arrived at the current Quanta
			{
				while(currentQuanta-1 != newArrivalArray[j].arrival_time-1 || currentQuanta-1 < newArrivalArray[j].arrival_time-1) //So we 
				{
                    //arrayFCFSOrder[currentQuanta] = '-';
					newArrivalArray[j].actual_start_time = currentQuanta + 1;
					currentQuanta++;

				}	
                arrayFCFSOrder[currentQuanta] = '-';
                newArrivalArray[j].end_time = currentQuanta;
                size++;
			}

            if (currentQuanta > MAX_QUANTA - 1) {
                break;
            }

		}
	
    for (int i = 0; i < CHAR_ARRAYMAX; i++) {
       
            printf("%c", arrayFCFSOrder[i]);
        
    }
    free(arrayFCFSOrder);
    
    // Size is now the # of processes that completed its process 
    printf("\nAverage response time: %.2f\n", calAverageResponse2(newArrivalArray, size));
    printf("Average waiting time: %.2f\n", calAverageWaiting(newArrivalArray, size));
    printf("Average turnaround time: %.2f\n", calAverageTurnaround(newArrivalArray, size));
	printf("Throughput: %.d\n", size);

    return arrayFCFSOrder;
}
