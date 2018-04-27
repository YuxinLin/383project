#ifndef FCFS
#define FCFS
#include "process_producer.h"


float avgTurnaround(struct process* list, int size);
float avgWaiting(struct process* list, int size);
float avgResponse(struct process* list, int size);
/* Function: getFCFS
*  currently used first serve first come
*/
//char* getFCFS(struct process** plist, int* size);
//void printFCFS(struct process* plist);
char* firstComeFirstServed(struct process* plist);

#endif

