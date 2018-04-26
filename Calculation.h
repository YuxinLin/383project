#ifndef Calculation
#define Calculation

/*
* Functions to calculate different processes data
*/
float calAverageTurnaround(struct process plist[], int size);
float calAverageWaiting(struct process plist[], int size);
float calAverageResponse(struct process plist[], int size);
float calAverageResponse2(struct process plist[], int size);
int calThroughput(char charArray[], int size);

#endif