//
//  seller.h
//  concert
//
//  Created by gladet on 5/1/18.
//  Copyright © 2018 gladet. All rights reserved.
//

#ifndef seller_h
#define seller_h

/**
 * Creates a new seller.
 *
 * @param type The type of seller-- either 'H', 'M', or 'L'
 * @param index The index of this particular seller
 *
 * @return A pointer to the pthread_t thread ID created by this method
 */
pthread_t* newSeller(char type, int index);

/**
 * Callback function for a high-priced seller
 */
void *highSeller(void* arg);

/**
 * Callback function for a medium-priced seller
 */
void *mediumSeller(void* arg);

/**
 * Callback function for a low-priced seller
 */
void *lowSeller(void* arg);

#endif /* seller_h */
