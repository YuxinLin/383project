#ifndef Calculation
#define Calculation

/*revision
 *Peng added new version of calculation for tat / wait time /response time
 */
//turnaround time
float AverageTAT(struct process jobs_list[], int size);
float AverageTATPre(struct process jobs_list[], int size);
//waiting time
float AverageWT(struct process jobs_list[], int size);
float AverageWTPre(struct process jobs_list[], int size);
//response time
float AverageRT(struct process jobs_list[], int size);
/*
 *end of Peng's revision
 */

/*
* Functions to calculate different processes data
*/
float calAverageTurnaround(struct process plist[], int size);
float calAverageWaiting(struct process plist[], int size);
float calAverageResponse(struct process plist[], int size);
float calAverageResponse2(struct process plist[], int size);
int calThroughput(char charArray[], int size);

#endif
