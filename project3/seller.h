#ifndef SELLER
#define SELLER

#include "customer.h"

///Seller struct consists of the sellers name, sales price - not used, and the beginning of the queue for customers
struct seller{
	///name consist of seller price e.g. H and the other 2 characters depending on number of seller
	char name[3];

	///1 = L; 2 = M; 3 = H - not currently used
	int sales_price;

	/// Start of customer queue
	customer* start_queue;
};

typedef struct seller seller;

/**
 * print_sellers prints a list of - their name and their sales price
 * @param list   list of sellers
 * @param length the number of sellers
 */
void print_sellers(seller* list, int length);

/**
 * sell_seats function to run in threads that will simulate a seller selling tickets to customers in queue
 * @param seat_seller pinter to seller
 */
void* sell_seats(void* seat_seller);

/**
 * print_seats() prints the seat slots
 */
void print_seats();

/**
 * print_stats() when all seats are taken
 */
void print_stats();
#endif