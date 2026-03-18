#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct pnode {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
    int inHeap;
    struct pnode *link;
} process;

typedef struct queue {
    process *p;
    struct queue *link;
} queue;

typedef struct gnode {
    int p_id;
    int start;
    int end;
    struct gnode *link;
} gnode;

typedef struct MinHeap {
    process **arr;
    int size;
    int capacity;
} MinHeap;

void addProcess();
void sortArrival();
void resetAllProcesses();
void fcfsScheduling();
void rrScheduling();
void srtfScheduling();
void addGantt(int p_id, int start, int end);
void viewGanttChart();

void rrEnqueue(process *p);
process* rrDequeue();
void resetRR();

void preemptProcess();
void viewReadyQueue();

MinHeap* createHeap(int capacity);
void insertHeap(MinHeap* h, process* p);
process* extractMin(MinHeap* h);
void heapifyUp(MinHeap* h, int idx);
void heapifyDown(MinHeap* h, int idx);
void freeHeap(MinHeap* h);

#endif