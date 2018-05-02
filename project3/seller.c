/*
 * seller.c
 *
 * Contains functions related to the ticket sellers;
 * creates new seller threads to serve customers
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "concert.h"
#include "customer.h"

#define HIGH 'H'
#define MEDIUM 'M'
#define LOW 'L'

void* highSeller(void*);
void* mediumSeller(void*);
void* lowSeller(void*);

int sellerIndex = 0;

const int HIGH_SELLER_MIN = 1;
const int HIGH_SELLER_MAX = 2;
const int MID_SELLER_MIN = 2;
const int MID_SELLER_MAX = 4;
const int LOW_SELLER_MIN = 4;
const int LOW_SELLER_MAX = 7;

/**
 * Creates a new seller.
 *
 * @param type The type of seller-- either 'H', 'M', or 'L'
 * @param index The index of this particular seller
 *
 * @return A pointer to the pthread_t thread ID created by this method
 */
pthread_t* newSeller(char type, int index)
{
    // Initialize the new thread and its components
    pthread_t sellerThreadID;
    pthread_attr_t sellerAttr;
    pthread_attr_init(&sellerAttr);
    // Prepare an int pointer to be sent to the callback functions
    int* id = malloc(sizeof(int));
    *id = index;
    // Create the pointer with the callback function corresponding
    // to this seller's type
    if(type == HIGH) {
        pthread_create(&sellerThreadID, &sellerAttr, highSeller, id);
    } else if(type == MEDIUM) {
        pthread_create(&sellerThreadID, &sellerAttr, mediumSeller, id);
    } else if(type == LOW) {
        pthread_create(&sellerThreadID, &sellerAttr, lowSeller, id);
    } else {
        return NULL;
    }
    return &sellerThreadID;
}

/**
 * Callback function for a high-priced seller
 */
void *highSeller(void* arg)
{
    int id = *((int *) arg);
	//int seller = sellerIndex++;
	int seller = sellerIndex;
	sellerIndex = (sellerIndex + 1) % TOTAL_QUEUES;
    
    while(!isFinished(getConcert())) {
        int customer = getCustomer(seller);
        if(customer != -1) {
            int* assignedSeat = nextFrontSeat();
            char event[99];
            sprintf(event, "Seller H%d sold row %d, seat %d to customer %d.", id, assignedSeat[0], assignedSeat[1], customer);
            setCustomer(getConcert(), assignedSeat, customer);
            printEvent(event);
            sleep(HIGH_SELLER_MIN + rand() % (HIGH_SELLER_MAX - HIGH_SELLER_MIN));
            sprintf(event, "Customer %2d purchase complete. Seller H%d ready.", customer, id);
            printEvent(event);
        }
    }
    int customer = getCustomer(seller);
    while(customer != -1) {
        char event[99];
        sprintf(event, "Customer %d left.", customer);
        printEvent(event);
        customer = getCustomer(seller);
    }
	
	return NULL;
}

/**
 * Callback function for a medium-priced seller
 */
void *mediumSeller(void* arg)
{
    int id = *((int*) arg);
	//int seller = sellerIndex++;
	int seller = sellerIndex;
	sellerIndex = (sellerIndex + 1) % TOTAL_QUEUES;
	
    while(!isFinished(getConcert())) {
        int customer = getCustomer(seller);
        if(customer != -1) {
            int* assignedSeat = nextMiddleSeat();
            char event[99];
            sprintf(event, "Seller M%d sold row %d, seat %d to customer %d.", id, assignedSeat[0], assignedSeat[1], customer);
            setCustomer(getConcert(), assignedSeat, customer);
            printEvent(event);
            sleep(MID_SELLER_MIN + rand() % (MID_SELLER_MAX - MID_SELLER_MIN));
            sprintf(event, "Customer %2d purchase complete. Seller M%d ready.", customer, id);
            printEvent(event);
        }
    }
    int customer = getCustomer(seller);
    while(customer != -1) {
        char event[99];
        sprintf(event, "Customer %d left.", customer);
        printEvent(event);
        customer = getCustomer(seller);
    }
	
	return NULL;
}

/**
 * Callback function for a low-priced seller
 */
void *lowSeller(void* arg)
{
    int id = *((int*) arg);
	//int seller = sellerIndex++;
	int seller = sellerIndex;
	sellerIndex = (sellerIndex + 1) % TOTAL_QUEUES;
	
    while(!isFinished(getConcert())) {
        int customer = getCustomer(seller);
        if(customer != -1) {
            int* assignedSeat = nextBackSeat();
            char event[99];
            sprintf(event, "Seller L%d sold row %d, seat %d to customer %d.", id, assignedSeat[0], assignedSeat[1], customer);
            setCustomer(getConcert(), assignedSeat, customer);
            printEvent(event);
            sleep(LOW_SELLER_MIN + rand() % (LOW_SELLER_MAX - LOW_SELLER_MIN));
            sprintf(event, "Customer %2d purchase complete. Seller L%d ready.", customer, id);
            printEvent(event);
        }
    }
    int customer = getCustomer(seller);
    while(customer != -1) {
        char event[99];
        sprintf(event, "Customer %d left.", customer);
        printEvent(event);
        customer = getCustomer(seller);
    }
	
	return NULL;
}

