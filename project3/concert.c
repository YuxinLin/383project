/*
 * concert.c
 *
 * Represents the concert hall-- data on which seats are taken,
 * and on how many tickets are left.
 */

/*revision
 *void printConcert(Concert* c) -> add return type void
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "concert.h"

const int SEATS_ACROSS = SEAT_WIDTH;
const int SEAT_ROWS = SEAT_HEIGHT;

// Global variable; concert singleton
Concert* concert;

time_t startTime;
pthread_mutex_t printMutex;
pthread_mutex_t concertMutex;

/**
 * Creates a new concert.
 *
 * @return A pointer to the Concert struct created by this method
 */
Concert* newConcert()
{
    Concert* newConcert = (Concert*) malloc(sizeof(Concert));
    (*newConcert).tickets = SEATS_ACROSS * SEAT_ROWS;
    (*newConcert).seats = malloc(sizeof(bool) * (*newConcert).tickets);
    (*newConcert).customer = malloc(sizeof(int) * (*newConcert).tickets);
    int i;
    int j;
    for(i = 0; i < SEAT_ROWS; i++) {
        for(j = 0; j < SEATS_ACROSS; j++) {
            (*newConcert).seats[i][j] = false;
        }
    }
    return newConcert;
}

/**
 * Sets the concert singleton
 *
 * @param c A pointer to the Concert struct that should be the new singleton
 */
void setConcert(Concert* c)
{
    concert = c;
    time(&startTime);
    pthread_mutex_init(&printMutex, NULL);
    pthread_mutex_init(&concertMutex, NULL);
}

/**
 * Returns the concert singleton
 *
 * @return A pointer to the current concert singleton
 */
Concert* getConcert()
{
    return concert;
}

/**
 * Returns the total number of tickets left in a concert
 *
 * @param c A pointer to the concert
 *
 * @return The number of tickets left to be sold
 */
int getTicketCount(Concert* c)
{
	return (*c).tickets;
}

/**
 * Determines whether a concert has been sold out or the selling window has ended.
 * 
 * @param c A pointer to the concert
 *
 * @return true if the concert has no more tickets; false otherwise
 * @return true if the time elapsed is 60mins or more; false otherwise
 */
bool isFinished(Concert* c)
{
   
    if(getTicketCount(c) <= 0){
	return true;
    }
    else{
	time_t now;
	time(&now);
        double time = difftime(now, startTime);
        return time >= 60;
    }
}

void printEvent(char event[99])
{
    pthread_mutex_lock(&printMutex);
    
    time_t now;
    time(&now);
    double time = difftime(now, startTime);
    int min = 0;
    int sec = (int) time;
    while(sec >= 60) {
        min++;
        sec -= 60;
    }
    printf("%02d:%02d | %s\n", min, sec, event);
    
    pthread_mutex_unlock(&printMutex);
}

int* nextFrontSeat()
{
    pthread_mutex_lock(&concertMutex);
    
    int* seat = malloc(sizeof(int) * 2);
    
    int i;
    int j;
    bool found = false;
    for(i = 0; i < SEAT_ROWS && !found; i++) {
        for(j = 0; j < SEATS_ACROSS && !found; j++) {
            if(!(*concert).seats[i][j]) {
                (*concert).seats[i][j] = true;
                found = true;
                seat[0] = i;
                seat[1] = j;
                (*concert).tickets--;
            }
        }
    }
    
    pthread_mutex_unlock(&concertMutex);
    
    return seat;
}

int* nextMiddleSeat()
{
    pthread_mutex_lock(&concertMutex);
    
    int* seat = malloc(sizeof(int) * 2);
    
    int base = (SEAT_ROWS - 1) / 2;
    int offset = 0;
    bool found = false;
    while(!found) {
        int row = base + offset;
        int i;
        for(i = 0; i < SEATS_ACROSS && !found; i++) {
            if(!(*concert).seats[row][i]) {
                (*concert).seats[row][i] = true;
                found = true;
                seat[0] = row;
                seat[1] = i;
                (*concert).tickets--;
            }
        }
        if(offset <= 0) {
            offset--;
            offset = -offset;
        } else {
            offset = -offset;
        }
    }
    
    pthread_mutex_unlock(&concertMutex);
    
    return seat;
}

int* nextBackSeat()
{
    pthread_mutex_lock(&concertMutex);
    
    int* seat = malloc(sizeof(int) * 2);
    
    int i;
    int j;
    bool found = false;
    for(i = SEAT_ROWS - 1; i >= 0 && !found; i--) {
        for(j = 0; j < SEATS_ACROSS && !found; j++) {
            if(!(*concert).seats[i][j]) {
                (*concert).seats[i][j] = true;
                found = true;
                seat[0] = i;
                seat[1] = j;
                (*concert).tickets--;
            }
        }
    }
    
    pthread_mutex_unlock(&concertMutex);
    
    return seat;
}

void setCustomer(Concert* c, int* seat, int customer)
{
    //seat[0] holds the row index
    //seat[1] holds the column index
    (*c).customer[seat[0]][seat[1]] = customer;
}

void printConcert(Concert* c)
{
    int i;
    int j;
    printf("\n+");
    for(i = 0; i < SEATS_ACROSS; i++) {
        if(i == 0) {
            printf("---");
        } else {
            printf("----");
        }
    }
    printf("+\n");
    for(i = 0; i < SEAT_ROWS; i++) {
        printf("|");
        for(j = 0; j < SEATS_ACROSS; j++) {
            if(j != 0) {
                printf(" ");
            }
	    printf("%3d", (*c).customer[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for(i = 0; i < SEATS_ACROSS; i++) {
        if(i == 0) {
            printf("---");
        } else {
            printf("----");
        }
    }
    printf("+\n");
}

