#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include "seller.h"
#include "customer.h"
#include "shared.h"
#include "algorithms.h"


#define MINUTES 60 

struct itimerval sellerTimer;

void wake_up(){
	pthread_mutex_lock(&seat_access);
	start = true;
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&seat_access);
}

void timerHandler();

int main(int argc, char *argv[]){

	if(argc < 2) {
        NUM_OF_CUSTOMERS = 15; // default customer size
        printf("\nNo N Given. Using default size N = 5\n\n");
    } else {
        NUM_OF_CUSTOMERS= atoi(argv[1]);
        printf("\nNumber of customers : %d\n\n", NUM_OF_CUSTOMERS);
    }

	// Initilize total sell counts for L, M and H sellers
	totalL_sold = 0;
	totalM_sold = 0;
	totalH_sold = 0;

	//Initilization of variables
	int seed = time(NULL);
	srand(seed);
	start = false;

	// Set timer to exit at 60 minutes
    sellerTimer.it_value.tv_sec = MAX_MINUTES;
    setitimer(ITIMER_REAL, &sellerTimer, NULL);
    time(&startTime);

	time_t currentTime; // type calender time
    time(&currentTime);
    int sec = (int) difftime(currentTime, startTime);

	seat_map = malloc(TOTAL_SEATS * (sizeof(char) * 4)); //100 seats can store 4 each seat
 	memset(seat_map, 45, TOTAL_SEATS * (sizeof(char) * 4));
	seller* h_sellers = malloc(sizeof(seller) * NUM_OF_H_SELLERS);//create h_sellers
	seller* m_sellers = malloc(sizeof(seller) * NUM_OF_M_SELLERS);//create m_sellers
	seller* l_sellers = malloc(sizeof(seller) * NUM_OF_L_SELLERS);//create l_sellers
	filled_seats = 0;

	//Setting and threads
	seat_access = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
	cond = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
	pthread_t h_seller_thread;
	pthread_t m_sellers_thread[3]; 
	pthread_t l_sellers_thread[6]; 

	//For the future broadcast call to restart threads

	// Populate high seller
	for(int i = 0; i < NUM_OF_H_SELLERS; i++){

		if(i < 10){
			h_sellers[i].name[0] = 'H'; 
			h_sellers[i].name[1] = (char) (i + '0'); // seller id
			h_sellers[i].name[2] = (char) (i + 48);
		}
		else{
			h_sellers[i].name[0] = 'H';
			h_sellers[i].name[1] = (char) (i + '0');
			h_sellers[i].name[2] = '2';
		}
		make_customers(&(h_sellers[i].start_queue), NUM_OF_CUSTOMERS);
		h_sellers[i].sales_price = 3;
	}

	//Populate all medium sellers
	for(int i = 0; i < NUM_OF_M_SELLERS; i++){

		if(i < 10){
			m_sellers[i].name[0] = 'M';
			m_sellers[i].name[1] = (char) (i + '0');
			m_sellers[i].name[2] = (char) (i + 48);
		}
		else{
			m_sellers[i].name[0] = 'M';
			m_sellers[i].name[1] = (char) (i + '0');
			m_sellers[i].name[2] = '2';
		}
		make_customers(&(m_sellers[i].start_queue), NUM_OF_CUSTOMERS);
		m_sellers[i].sales_price = 2;
	}

	// Populate all low sellers
	for(int i = 0; i < NUM_OF_L_SELLERS; i++){

		if(i < 10){
			l_sellers[i].name[0] = 'L'; 
			l_sellers[i].name[1] = i + '0';
			l_sellers[i].name[2] = (char) (i + 48);
		}
		else{
			l_sellers[i].name[0] = 'L';
			l_sellers[i].name[1] = i + '0';
			l_sellers[i].name[2] = '2';
		}
		make_customers(&(l_sellers[i].start_queue), NUM_OF_CUSTOMERS);
		l_sellers[i].sales_price = 1;
	}


	printf("\n----------------QUEUE H seller-------------------------------\n");
	print_customers(h_sellers[0].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE FIRST M seller-------------------------------\n");
	print_customers(m_sellers[0].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE SECOND M seller-------------------------------\n");
	print_customers(m_sellers[1].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE THIRD M seller-------------------------------\n");
	print_customers(m_sellers[2].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE FIRST L seller-------------------------------\n");
	print_customers(l_sellers[0].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE SECOND L seller-------------------------------\n");
	print_customers(l_sellers[1].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE THIRD L seller-------------------------------\n");
	print_customers(l_sellers[2].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE FOURTH L seller-------------------------------\n");
	print_customers(l_sellers[3].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE FIFTH L seller-------------------------------\n");
	print_customers(l_sellers[4].start_queue, NUM_OF_CUSTOMERS);

	printf("\n----------------QUEUE SIXTH L seller-------------------------------\n");
	print_customers(l_sellers[5].start_queue, NUM_OF_CUSTOMERS);

    printf("\n\nTimer- %1d:%02d\n", (int) (sec / 60), (int) sec % 60); // Print initial time stamp

	// Create seller M threads
	for(int i = 0; i < NUM_OF_M_SELLERS; i++){ 
		// arguments(structure, NULL, function that runs in thread, argument to sell_seats function)
		pthread_create(&(m_sellers_thread[i]), NULL, sell_seats, (void*) &m_sellers[i]);
	}
	// Create seller L threads
	for(int i = 0; i < NUM_OF_L_SELLERS; i++){
		pthread_create(&(l_sellers_thread[i]), NULL, sell_seats, (void*) &l_sellers[i]);
	}
	// Create seller H thread
	pthread_create(&h_seller_thread, NULL, sell_seats, h_sellers);

	wake_up();
    signal(SIGALRM, timerHandler); // Set signal to listen for when time hits 60 seconds, then call timerHandler function

	//join all threads
	pthread_join(h_seller_thread, NULL);
	for(int i = 0; i < NUM_OF_L_SELLERS; i++){
		pthread_join(l_sellers_thread[i], NULL);
	}
	for(int i = 0; i < NUM_OF_M_SELLERS; i++){
		pthread_join(m_sellers_thread[i], NULL);
	}
	return 0;
}

void timerHandler(int signal){
	print_stats();
}
