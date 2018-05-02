/*
 * customer.c
 *
 * Contains functions related to the concert's customers;
 * creates new customer threads, who then buy tickets
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "concert.h"

// Defines the last time at which a customer might arrive
// Simulation: minutes
//   Realtime: seconds
const int LAST_CUSTOMER_ARRIVAL_TIME = 59;

int customerCounter = 0;
//int (*queue)[TOTAL_QUEUES];
int** queue;
int lineFront[TOTAL_QUEUES];
int num_customers_per_queue;

pthread_mutex_t queueMutex;

void* customer(void*);
/**
 * Creates a new customer.
 *
 * @param sellerIndex The index of the line to which this customer will go
 * 
 * @return A pointer to the pthread_t thread ID of the customer that was created
 */
pthread_t* newCustomer(int sellerIndex)
{
    pthread_t customerThreadID;
    pthread_attr_t customerAttr;
    pthread_attr_init(&customerAttr);
    int* id = malloc(sizeof(int));
	*id = sellerIndex; //check seller index
    pthread_create(&customerThreadID, &customerAttr, customer, id);
    return &customerThreadID;
}

/**
 * Callback function for a customer
 */
void *customer(void* arg)
{
    sleep(1 + rand() % LAST_CUSTOMER_ARRIVAL_TIME);
    int me = customerCounter++;
    int id = *((int*) arg);
    
    pthread_mutex_lock(&queueMutex);
    
    int i;
    for(i = 0; queue[i][id] != -1; i++) { }
    queue[i][id] = me;
    char event[99];
    sprintf(event, "Customer %2d arrives (line %d, position %d)", me, id, i);
    printEvent(event);
    
    pthread_mutex_unlock(&queueMutex);
	
	return NULL;
}

void initializeCustomerQueues(int maxCustomers)
{
	num_customers_per_queue = maxCustomers;
    pthread_mutex_init(&queueMutex, NULL);
	//queue = malloc(sizeof(int) * (maxCustomers + 1) * getCustomer);
	//queue = malloc(sizeof(int) * (maxCustomers + 1) * TOTAL_QUEUES);
	//queue = (int**) malloc(sizeof(int*) * num_customers_per_queue);
	queue = (int**) malloc(sizeof(int*) * (maxCustomers + 1));
	for(int i = 0; i < maxCustomers + 1; i++) {
	//for(int i = 0; i < num_customers_per_queue; i++) {
		queue[i] = (int*) malloc(sizeof(int) * TOTAL_QUEUES);
	}
	
    int i;
    int j;
    for(i = 0; i < maxCustomers + 1; i++) {
        for(j = 0; j < TOTAL_QUEUES; j++) {
            queue[i][j] = -1;
        }
    }
    for(i = 0; i < TOTAL_QUEUES; i++) {
        lineFront[i] = 0;
    }
}

int getCustomer(int queueIndex)
{
    pthread_mutex_lock(&queueMutex);
    
    int customerIndex = queue[lineFront[queueIndex]][queueIndex];
    if(customerIndex != -1) {
			//lineFront[queueIndex]++;
			lineFront[queueIndex] = (lineFront[queueIndex] + 1) % (num_customers_per_queue + 1);
    }
    
    pthread_mutex_unlock(&queueMutex);
    
    return customerIndex;
}

