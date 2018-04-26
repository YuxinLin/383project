#ifndef SRT
#define SRT

// this function will complete the processes in the Shortest remaining time order and will return a string as a result and showing in what order the processes were completed
char* SRT(struct process* plist)

// checks whether all the processes inputted have been completed or not
bool checkPCompletion(struct process* plist)
