#ifndef SHARED
#define SHARED
#include "pthread.h"
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

///Total Number of seats that can be sold
#define TOTAL_SEATS 100	
// Total minutes to sell
#define MAX_MINUTES 60 
#define NUM_OF_H_SELLERS 1
#define NUM_OF_M_SELLERS 3
#define NUM_OF_L_SELLERS 6

struct itimerval sellerTimer;

/// Restricts access to seats
pthread_mutex_t seat_access; 

pthread_cond_t cond;

time_t startTime;

///Counts how many seats are filled - semaphore
int filled_seats; 

///Actual map of location of seats - inc 0, 3, 6, 9 due to 3 chars being stored per seat
char* seat_map; 

///When to start selling
bool start; 

int NUM_OF_CUSTOMERS;

int totalL_sold; // total tickets L seller sold
int totalM_sold; // total tickets M seller sold
int totalH_sold; // total tickets M seller sold


#endif