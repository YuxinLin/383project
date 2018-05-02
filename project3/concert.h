//
//  concert.h
//  concert
//
//  Created by gladet on 5/1/18.
//  Copyright © 2018 gladet. All rights reserved.
//


#ifndef concert_h
#define concert_h

#include "stdbool.h"

#define SEAT_WIDTH 10
#define SEAT_HEIGHT 10

#define TOTAL_QUEUES 10

// Concert struct
typedef struct {
	int tickets;               // Total number of tickets yet to be sold
	bool (*seats)[SEAT_WIDTH]; // Record of which seats have been taken
	int (*customer)[SEAT_WIDTH];// Record of which customers are in which seats
} Concert;


/**
 * Creates a new concert.
 *
 * @return A pointer to the Concert struct created by this method
 */
Concert* newConcert();

/**
 * Sets the concert singleton
 *
 * @param c A pointer to the Concert struct that should be the new singleton
 */
void setConcert(Concert* c);

/**
 * Returns the concert singleton
 *
 * @return A pointer to the current concert singleton
 */
Concert* getConcert();

/**
 * Returns the total number of tickets left in a concert
 *
 * @param c A pointer to the concert
 *
 * @return The number of tickets left to be sold
 */
int getTicketCount(Concert* c);

/**
 * Determines whether a concert has been sold out or the selling window has ended.
 *
 * @param c A pointer to the concert
 *
 * @return true if the concert has no more tickets; false otherwise
 * @return true if the time elapsed is 60mins or more; false otherwise
 */
bool isFinished(Concert* c);

void printEvent(char event[99]);

int* nextFrontSeat();

int* nextMiddleSeat();

int* nextBackSeat();

void setCustomer(Concert* c, int* seat, int customer);

void printConcert(Concert* c);

#endif /* concert_h */
