#ifndef RR
#define RR

#include "process_producer.h"

/**
 * RoundRobin: calculates the run time for an array of processes if each process got exactly 1 quanta time slice. The value is stored in the runtime variable
 * array should be sorted by arrival time
 * @param plist pass in a list of structs - must be size of the NUM_PROCESS macro from process_producer.h
 */
void RoundRobin(struct process* plist);

#endif