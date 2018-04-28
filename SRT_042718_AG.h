
#include "process_producer.h"
#include "Sorter.h"
#include <math.h>

#ifndef SRT_HPP
#define SRT_HPP

// this function will complete the processes in the Shortest remaining time order and will return a string as a result and showing in what order the processes were completed
void SRT(struct process* plist);
//sort algo for dynamic size
void quicksort(struct process* number[], int first,int last);
// romove first job in candidate queue
void remove_first(struct process* number[], int* num);
// while processing job, iniend_time will be increment
void update_initend_time(struct process* candidates[], int num);

#endif
