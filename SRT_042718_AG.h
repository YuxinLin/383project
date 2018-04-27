
#include "process_producer.h"
#include "Sorter.h"
#include <math.h>

#ifndef SRT_HPP
#define SRT_HPP

//typedef process 

// this function will complete the processes in the Shortest remaining time order and will return a string as a result and showing in what order the processes were completed
char* SRT(struct process* plist);

// checks whether all the processes inputted have been completed or not
//bool checkPCompletion(struct process* plist);

void SortByAG(struct process** plist, int num);
void SortByRecAG(struct process** plist, int num, int range_lo, int range_hi);
void update_initend_time(struct process* candidates[], int num);
void printSRT(struct process* plist);

#endif
