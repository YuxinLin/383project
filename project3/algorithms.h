#ifndef ALGORITHMS
#define ALGORITHMS
/**
 * sort sorts objects from smallest to largest. Objects must have an integer for comparison
 * @param start_obj beginning of the array containing the list of objects
 * @param int_comp  pointer to the first integer contained in the object. this is the integer to compare
 * @param obj_size  size of each object/struct in the array
 * @param length    total amount of objects in the array
 */
void sort(void* start_obj, int* int_comp, int obj_size, int length);

/**
 * swap swaps 2 objects utilizing memcpy and memmove. Currently only callable was within algorithms.c
 * @param obj_a  pointer to first object - must be the start of the object
 * @param obj_b  pointer to the second object - must be start of the object
 * @param size_a size of object a - MUST BE SAME SIZE AS OBJECT B
 * @param size_b size of object b - MUST BE SAME SIZE AS OBJECT A
 */
static void swap(void* obj_a, void* obj_b, int size_a, int size_b);

/**
 * sort_rec recursive call from sort function. Uses quick sort to sort objects. It is static so it can only be called from algorithms.c
 * @param start_obj beginning of the array containing the list of objects
 * @param int_comp  pointer to the first integer contained in the object. this is the integer to compare
 * @param obj_size  size of each object/struct in the array
 * @param length    total amount of objects in the array
 */
static void sort_rec(void* start_obj, int* int_comp, int obj_size, int length);

#endif