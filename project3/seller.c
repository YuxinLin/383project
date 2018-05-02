#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shared.h"
#include "seller.h"


void print_sellers(seller* list, int length){
	char* p; 
	char* i_p =(char*) &list[0].sales_price;
	for(int x = 0; x < length; x++){
		p = (char*) &list[x];
		printf("%c-%c%c Sales Price: %d\n", *p, *(p + 1), *(p + 2), *( (int*) i_p) );
		i_p += (sizeof(seller));
	}
	return;
}

void* sell_seats(void* seat_seller){

	// Wait for seat access then unlock 
	while(!start) pthread_cond_wait(&cond, &seat_access);
	pthread_mutex_unlock(&seat_access);
	seller* seat_seller_s = (seller*) seat_seller;
	time_t after;
	time(&after);
	// Loop through the queue
   	for (int i = 0; i < NUM_OF_CUSTOMERS; i++)  {
    	time(&after);
    	int currentTime = (int) difftime(after, startTime) % 60;
		while(currentTime < seat_seller_s->start_queue->arrival_time && filled_seats < TOTAL_SEATS){
			sleep(1);
			time(&after);
			currentTime = (int) difftime(after, startTime) % 60;
		}
		printf("CTime: %d\n", currentTime);
		char currentCustomer[2];
 		currentCustomer[1] = seat_seller_s->start_queue->customer_id[1]; 
 		currentCustomer[0] = seat_seller_s->start_queue->customer_id[0];
 		printf("\nCustomer ID %c%c Seller %c%c arrives\n", seat_seller_s->start_queue->customer_id[0], seat_seller_s->start_queue->customer_id[1], 
			seat_seller_s->name[0],
			seat_seller_s->name[1]);


		if (seat_seller_s->sales_price == 1) {
			// L seller
			printf("\nCustomer %c%c of Seller %c%c being served\n", currentCustomer[0], currentCustomer[1], (*seat_seller_s).name[0], (*seat_seller_s).name[1]);

			sleep((rand() % 4) + 4); // Sell for 4,5,6 or 7 minutes
			pthread_mutex_lock(&seat_access); // bad idea to have multiple locks??
			// Sell seat starting with row 10 and work towards the front
			for(int x = 396; x > 0; x -= 4){
				if(*(seat_map + x) == (char) 45){ // (char) 45 = '-'
					totalL_sold++; // Add to L sold count
					
                    time(&after);
                    int aftertime = (int) difftime(after, startTime);
					
					*(seat_map + x) = (*seat_seller_s).name[0];
					*(seat_map + x + 1) = (*seat_seller_s).name[1];
					//TODO: ADD SLOT FOR CUSTOMER
					*(seat_map + x + 2) = currentCustomer[0];
					*(seat_map + x + 3) = currentCustomer[1]; 	
					filled_seats++;				
					print_seats();
					if(filled_seats >= TOTAL_SEATS) print_stats();

					printf("\n....%1d:%02d\n\n", aftertime / 60, aftertime % 60);
					printf("\nCustomer %c%c of seller %c%c purchased a ticket at %1d:%02d ", currentCustomer[0], currentCustomer[1],
						(*seat_seller_s).name[0], (*seat_seller_s).name[1], aftertime / 60, aftertime % 60);
					seat_seller_s->start_queue++;
					break;
				}
			}

		} else if (seat_seller_s->sales_price == 2) {
			// M seller

			printf("\nCustomer %c%c of Seller %c%c being served\n", currentCustomer[0], currentCustomer[1], (*seat_seller_s).name[0], 
				(*seat_seller_s).name[1]);

			sleep((rand() % 3) + 2); // Sell for 2,3 or 4 minutes
			pthread_mutex_lock(&seat_access); 
			// Sell seat starting with row 5 then 6 then 4 then 7
			int startingIndex = 160;
			int seatNotSold = 1;

			do {
				if (*(seat_map + startingIndex) == (char) 45) {
					totalM_sold++; // Add to M sold count

                    time(&after);
                    int aftertime = (int) difftime(after, startTime);
					
					*(seat_map + startingIndex) = (*seat_seller_s).name[0];
					*(seat_map + startingIndex + 1) = (*seat_seller_s).name[1];
					*(seat_map + startingIndex + 2) = currentCustomer[0];
					*(seat_map + startingIndex + 3) = currentCustomer[1];
					seat_seller_s->start_queue++;
					seatNotSold = 0; // Seat now sold
					filled_seats++;
					print_seats();
					if(filled_seats >= TOTAL_SEATS) print_stats();

					printf("\n....%1d:%02d\n\n", aftertime / 60, aftertime % 60);
					printf("\nCustomer %c%c of seller %c%c purchased a ticket at %1d:%02d ", currentCustomer[0], currentCustomer[1],
						(*seat_seller_s).name[0], (*seat_seller_s).name[1], aftertime / 60, aftertime % 60);
				}

				startingIndex += 4; // Increment by 4 in index
				// Switching for 5->6->4->7->3->8->2 -> 9->1->10 
				if (startingIndex == 240) { // Hit 7 after end of 6, go back to row 4
					startingIndex = 120; // Index of row 4
				} else if (startingIndex == 160) { // Hit row 5 after end of 4, go to row 7
					startingIndex = 240;
				} else if (startingIndex == 280) { // hit row 8 after end of 7, go to 3
					startingIndex = 80; 
				} else if (startingIndex == 120) { // Hit row 4 after end of 3, go to 8
					startingIndex = 280;
				} else if (startingIndex == 320) { // Hit row 9 after end of 8, go to 2
					startingIndex = 40;
				} else if (startingIndex == 80) { // Hit row 3 after end of 2, go to 9
					startingIndex = 320;
				} else if (startingIndex == 360) { // Hit row 10 after 9, go to 1
					startingIndex = 0;
				} else if (startingIndex == 40) { // Hit row 2 after 1, go to 10
					startingIndex = 360;
				}

		


			} while (seatNotSold);
		
		} else if (seat_seller_s->sales_price == 3) {
			// H Seller sales_price = 3

			printf("\nCustomer %c%c of Seller %c%c being served\n", currentCustomer[0], currentCustomer[1], (*seat_seller_s).name[0], 
				(*seat_seller_s).name[1]);
			sleep((rand() % 1) + 1); // Sell for 1 or 2 minutes
			pthread_mutex_lock(&seat_access); 
			// Sell seat starting with row 1 then worl towards the front
			for(int x = 0; x < (TOTAL_SEATS * 4); x += 4){
				if(*(seat_map + x) == (char) 45){
					totalH_sold++; // Add to H sold count

					
                    time(&after);
                    int aftertime = (int) difftime(after, startTime);

            		*(seat_map + x) = (*seat_seller_s).name[0];
					*(seat_map + x + 1) = (*seat_seller_s).name[1];
					*(seat_map + x + 2) = currentCustomer[0];
					*(seat_map + x + 3) = currentCustomer[1];
					seat_seller_s->start_queue++;
					filled_seats++;
					print_seats();
					if(filled_seats >= TOTAL_SEATS) print_stats();

					printf("\n....%1d:%02d\n\n", aftertime / 60, aftertime % 60);
					printf("\nCustomer %c%c of seller %c%c purchased a ticket at %1d:%02d ", currentCustomer[0], currentCustomer[1],
						(*seat_seller_s).name[0], (*seat_seller_s).name[1], aftertime / 60, aftertime % 60);
					break;
				}
			}
		}

		pthread_mutex_unlock(&seat_access);
	}
	
	pthread_exit(NULL);
}

void print_seats(){
	printf("\n\n");
	for(int x = 0; x < (TOTAL_SEATS * 4); x += 4){
		printf("|%c%c%c%c| ", seat_map[x], seat_map[x + 1], seat_map[x + 2], seat_map[x + 3]);
		if (x == 36 || x == 76 || x == 116 || x == 156 || x == 196 || x == 236 ||
			x == 276 || x == 316 || x == 356) {
			printf("\n");
		}
	}
	printf("\n\n");
}

void print_stats() {
	int totalCustomers = (NUM_OF_CUSTOMERS * 10);
	printf("\nTotal seats sold: %d", filled_seats);
	printf("\nTotal L seats sold: %d", totalL_sold);
	printf("\nTotal M seats sold: %d", totalM_sold);
	printf("\nTotal H seats sold: %d", totalH_sold);
	printf("\nTotal customers turned away: %d", totalCustomers - totalL_sold - totalM_sold - totalH_sold);
	printf("\n");
	exit(0); // Program finishes
}