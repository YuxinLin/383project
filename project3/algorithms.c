#include <stdio.h>
#include <stdlib.h>
#include "string.h"

static void swap(void* obj_a, void* obj_b, int size_a, int size_b){
	if(size_a != size_b) return;
	void* tmp = malloc(size_a);
	memcpy(tmp, obj_a, size_a);
	memmove(obj_a, obj_b, size_a);
	memmove(obj_b, tmp, size_a);
	free(tmp);
}

static void sort_rec(void* start_obj, int* int_comp, int obj_size, int length){
	//Set pointer to beginning of array
	if(length <= 1) return;
	int inc = 1;
	int pivot = length - 1;
	int p_loc = 0;
	char* beg_objb = (char*) start_obj;
	char* int_compb = (char*) int_comp;

	//printf("Comp1: %d Comp2: %d\n", *( (int*) (int_compb) ), *( (int*) (int_compb + (obj_size * length) ) ) );
	//printf("%d", *((int*)(beg_objb + (obj_size * length))))
	for(int x = 0; x < length; x++){
		if(pivot == p_loc) break;
		if( *( (int*) (int_compb + (obj_size * pivot)) ) < *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot  > p_loc){
			swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
			swap(&pivot, &p_loc, sizeof(int), sizeof(int));
			inc *= -1;
		}
		else if( *( (int*) (int_compb + (obj_size * pivot)) ) > *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot < p_loc){
			swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
			swap(&pivot, &p_loc, sizeof(int), sizeof(int));
			inc *= -1;
		}
		p_loc += inc;
	}
	sort_rec((beg_objb + (obj_size * pivot)), (int*) (int_compb + (obj_size * pivot)), obj_size, length - pivot);
	sort_rec(beg_objb, (int*) int_compb, obj_size, pivot);
	return;
}

void sort(void* start_obj, int* int_comp, int obj_size, int length){
	//Set pointer to beginning of array
	int inc = 1;
	int pivot = length - 1;
	int p_loc = 0;
	char* beg_objb = (char*) start_obj;
	char* int_compb = (char*) int_comp;

	//printf("Comp1: %d Comp2: %d\n", *( (int*) (int_compb) ), *( (int*) (int_compb + (obj_size * length) ) ) );
	//printf("%d", *((int*)(beg_objb + (obj_size * length))))
	for(int x = 0; x < length; x++){
		if(pivot == p_loc) break;
		if( *( (int*) (int_compb + (obj_size * pivot)) ) < *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot  > p_loc){
			swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
			swap(&pivot, &p_loc, sizeof(int), sizeof(int));
			inc *= -1;
		}
		else if( *( (int*) (int_compb + (obj_size * pivot)) ) > *( (int*) (int_compb + (obj_size * p_loc) ) ) && pivot < p_loc){
			swap((beg_objb + (obj_size * pivot)), (beg_objb + (obj_size * p_loc)), obj_size, obj_size);
			swap(&pivot, &p_loc, sizeof(int), sizeof(int));
			inc *= -1;
		}
		p_loc += inc;
	}
	//start at pivot
	sort_rec((beg_objb + (obj_size * pivot)), (int*) (int_compb + (obj_size * pivot)), obj_size, length - pivot);
	sort_rec(beg_objb, (int*) int_compb, obj_size, pivot);
	return;
}