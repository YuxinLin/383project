//
//  customer.h
//  concert
//
//  Created by gladet on 5/1/18.
//  Copyright © 2018 gladet. All rights reserved.
//

#ifndef customer_h
#define customer_h

void* customer(void*);
/**
 * Creates a new customer.
 *
 * @param sellerIndex The index of the line to which this customer will go
 *
 * @return A pointer to the pthread_t thread ID of the customer that was created
 */
pthread_t* newCustomer(int sellerIndex);

/**
 * Callback function for a customer
 */
void *customer(void* arg);

void initializeCustomerQueues(int maxCustomers);

int getCustomer(int queueIndex);

#endif /* customer_h */
