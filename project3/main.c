/*
 * The main method for assignment 3.
 * More specific code should be put in separate files
 * and grouped appropriately.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "tools.h"
#include "concert.h"
#include "seller.h"
#include "customer.h"

//#include "./tools.c"

//#include "./concert.c"
//#include "./customer.c"
//#include "./seller.c"

const int HIGH_SELLERS = 1;
const int MID_SELLERS = 3;
const int LOW_SELLERS = 6;

int main(int argc, char* argv[])
{
    if(argc != 2) // Test for the right number of command-line arguments
    { // If the count is wrong, print a usage mesage instead.
        printf("%s requires ONE command-line argument\n", argv[0]);
        printf("(the number of customers per seller).\n");
        return 0;
    }
    
    srand(time(0));
    setConcert(newConcert());
    
    // Convert the number of customers passed to the program to an
    // int for later use
    const int NUMBER_OF_CUSTOMERS = atoi(argv[1]);
    
    printf("Proceeding with %d customers per seller.\n", NUMBER_OF_CUSTOMERS);
    
    // Create the sellers so that there will be lines to go to
    int i;
    pthread_t sellerThread[HIGH_SELLERS + MID_SELLERS + LOW_SELLERS];
    for(i = 0; i < HIGH_SELLERS; i++) {
        sellerThread[i] = *newSeller('H', i);
    }
    for(i = 0; i < MID_SELLERS; i++) {
        sellerThread[HIGH_SELLERS + i] = *newSeller('M', i);
    }
    for(i = 0; i < LOW_SELLERS; i++) {
        sellerThread[HIGH_SELLERS + MID_SELLERS + i] = *newSeller('L', i);
    }

    // Create customers to go to the lines
    initializeCustomerQueues(NUMBER_OF_CUSTOMERS);
    for(i = 0; i < HIGH_SELLERS + MID_SELLERS + LOW_SELLERS; i++) {
        int j;
        for(j = 0; j < NUMBER_OF_CUSTOMERS; j++) {
            newCustomer(i);
        }
    }
    
    do {          // Loop to ensure that the program doesn't terminate before
        sleep(1); // all seats have been assigned
        char event[99];
        sprintf(event, "%3d tickets remain.", (*getConcert()).tickets);
        printEvent(event);
    } while(!isFinished(getConcert()));
    for(i = 0; i < HIGH_SELLERS + MID_SELLERS + LOW_SELLERS; i++) {
        pthread_join(sellerThread[i], NULL);
    }
    printConcert(getConcert());
    
    return 0;
}

