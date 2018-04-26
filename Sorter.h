#ifndef Sorter
#define Sorter

#include "process_producer.h"
#include <string.h>
/**
 * Sorts the structure list according to num - list must be same size as NUM_PROCESS macro
 * @param plist  list to sort
 * @param num   The input num decides what field to sort by 0 - arrival time; 1 - service time; 2 - priority; 3 - actual start time; 4 - end time
 */
void SortBy(struct process** plist, int num);

/**
 * Sort call called from main SortBy function. ONLY USED BY SortBy, should not be called by itself.
 * @param plist    list to be sorted
 * @param num      determines how to sort according to parent function
 * @param range_lo used to grab lower part of array
 * @param range_hi used to grab higher part of array
 */
void SortByRec(struct process** plist, int num, int range_lo, int range_hi);


/**
 * [PrintServiceTime - function for testing service time of process list]
 * @param plist process list to view service times
 */
void PrintServiceTime(struct process** plist);

/**
 * swaps 2 objects
 * @param objA      object a to swap
 * @param objB      object b to swap
 * @param size_objA size of obj a
 * @param size_objB size of obj b
 */
void swap(void* objA, void* objB, size_t size_objA, size_t size_objB);

#endif