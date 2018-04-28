#ifndef HPFA
#define HPFA
int findSize(struct process* plist, int priority);
void merge(struct process** queue, int left, int mid, int right);
void mergeSort(struct process** queue, int left, int right);
float AverageTAT(struct process* list, int size);
float AverageRT(struct process* list, int size);
float AverageWT(struct process* list, int size);
void nonpreHPF(struct process* plist);

#endif
