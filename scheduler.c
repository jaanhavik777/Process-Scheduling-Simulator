#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

queue *front = NULL;
queue *rear = NULL;
queue *rrFront = NULL;
queue *rrRear = NULL;
gnode *ghead = NULL;
int preemptRequested = 0;
int currentPID_SRTF = -1;
process *currentProc_SRTF = NULL;
MinHeap *globalHeap = NULL;
int rrActive = 0;

void addProcess()
{
    process *newP = malloc(sizeof(process));
    printf("Enter\n id\t arrival time\t burst time\n");
    scanf("%d %d %d", &newP->id, &newP->arrivalTime, &newP->burstTime);

    newP->remainingTime = newP->burstTime;
    newP->completionTime = 0;
    newP->turnaroundTime = 0;
    newP->waitingTime = 0;
    newP->inHeap = 0;

    queue *qnode = malloc(sizeof(queue));
    qnode->p = newP;
    qnode->link = NULL;

    if (front == NULL)
        front = rear = qnode;
    else {
        rear->link = qnode;
        rear = qnode;
    }

    printf("p%d is added\n", newP->id);
}

void sortArrival() {
    if (front == NULL || front->link == NULL) return;
    queue *sorted = NULL;
    queue *curr = front;
    while (curr != NULL) {
        queue *next = curr->link;
        if (sorted == NULL || curr->p->arrivalTime < sorted->p->arrivalTime) {
            curr->link = sorted;
            sorted = curr;
        } else {
            queue *temp = sorted;
            while (temp->link != NULL && temp->link->p->arrivalTime <= curr->p->arrivalTime) {
                temp = temp->link;
            }
            curr->link = temp->link;
            temp->link = curr;
        }
        curr = next;
    }
    front = sorted;
    queue *temp = front;
    while (temp->link != NULL)
        temp = temp->link;
    rear = temp;
}

void resetAllProcesses()
{
    queue *t = front;
    while (t != NULL) {
        process *p = t->p;
        p->remainingTime = p->burstTime;
        p->completionTime = 0;
        p->turnaroundTime = 0;
        p->waitingTime = 0;
        p->inHeap = 0;
        t = t->link;
    }
}

void fcfsScheduling()
{
    ghead = NULL;
    if (front == NULL) {
        printf("No process in queue\n");
        return;
    }
    sortArrival();

    queue *temp = front;
    int currentTime = 0;

    while (temp != NULL) {
        process *currP = temp->p;

        if (currentTime < currP->arrivalTime)
            currentTime = currP->arrivalTime;

        int start = currentTime;
        currentTime = currentTime + currP->burstTime;
        int end = currentTime;

        currP->completionTime = currentTime;
        currP->turnaroundTime = currP->completionTime - currP->arrivalTime;
        currP->waitingTime = currP->turnaroundTime - currP->burstTime;

        addGantt(currP->id, start, end);

        temp = temp->link;
    }
}

void rrEnqueue(process *p) {
    queue *node = malloc(sizeof(queue));
    node->p = p;
    node->link = NULL;
    if (rrRear == NULL) {
        rrFront = rrRear = node;
        return;
    }
    rrRear->link = node;
    rrRear = node;
}

process* rrDequeue() {
    if (rrFront == NULL)
        return NULL;

    queue *temp = rrFront;
    process *p = temp->p;
    rrFront = rrFront->link;
    if (rrFront == NULL)
        rrRear = NULL;

    free(temp);
    return p;
}

void resetRR() {
    while (rrFront != NULL) {
        queue *t = rrFront;
        rrFront = rrFront->link;
        free(t);
    }
    rrRear = NULL;
}

void rrScheduling() {
    ghead = NULL;
    rrActive = 1;

    if (front == NULL) {
        printf("No process in queue\n");
        rrActive = 0;
        return;
    }

    resetRR();

    int quantum;
    printf("Enter quantum time\n");
    scanf("%d", &quantum);

    int currentTime = 0;
    queue *t = front;

    while (t != NULL && t->p->arrivalTime == 0) {
        rrEnqueue(t->p);
        t = t->link;
    }

    while (rrFront != NULL || t != NULL) {

        if (rrFront == NULL) {
            currentTime = t->p->arrivalTime;
            rrEnqueue(t->p);
            t = t->link;
        }

        if (preemptRequested) {
            preemptRequested = 0;
            process *p_temp = rrDequeue();
            if (p_temp) {
                rrEnqueue(p_temp);
                printf("[RR] Manual preempt: rotated P%d → back of ready queue.\n", p_temp->id);
            }
        }

        process *p = rrDequeue();
        if (!p) continue;

        int exec = (p->remainingTime > quantum) ? quantum : p->remainingTime;
        int start = currentTime;
        currentTime += exec;
        int end = currentTime;

        addGantt(p->id, start, end);
        p->remainingTime -= exec;

        while (t != NULL && t->p->arrivalTime <= currentTime) {
            rrEnqueue(t->p);
            t = t->link;
        }

        if (p->remainingTime > 0) {
            rrEnqueue(p);
        }
        else {
            p->completionTime = currentTime;
            p->turnaroundTime = p->completionTime - p->arrivalTime;
            p->waitingTime = p->turnaroundTime - p->burstTime;
        }
    }

    rrActive = 0;
}

void preemptProcess()
{
    if (rrActive && rrFront != NULL) {
        process *p = rrDequeue();
        if (p) {
            rrEnqueue(p);
            printf("[Manual Preempt] RR: moved P%d to back of ready queue.\n", p->id);
            return;
        }
    }

    if (currentProc_SRTF != NULL) {
        preemptRequested = 1;
        printf("[Manual Preempt] SRTF: preemption requested for P%d (will take effect next time unit).\n", currentProc_SRTF->id);
        return;
    }

    printf("No ready process to preempt and no SRTF running process.\n");
}

void viewReadyQueue()
{
    if (rrFront == NULL) {
        printf("Ready Queue is empty (no process ready).\n");
        return;
    }

    queue *temp = rrFront;
    printf("Ready Queue -> ");
    while (temp != NULL) {
        printf("P%d(rem=%d) ", temp->p->id, temp->p->remainingTime);
        temp = temp->link;
    }
    printf("\n");
}

void srtfScheduling() {
    ghead = NULL;
    if (front == NULL) {
        printf("No process in queue\n");
        return;
    }

    MinHeap *heap = createHeap(10);
    globalHeap = heap;
    int time = 0, completed = 0, total = 0;
    queue *q = front;
    while (q != NULL) {
        total++;
        q = q->link;
    }

    int currentPID = -1;
    int startTime = 0;
    currentProc_SRTF = NULL;
    preemptRequested = 0;

    while (completed < total) {

        for (queue *t = front; t != NULL; t = t->link) {
            process *p = t->p;
            if (p->arrivalTime == time && p->remainingTime > 0) {
                insertHeap(heap, p);
            }
        }

        if (heap->size == 0) {
            if (currentPID != -1) {
                addGantt(currentPID, startTime, time);
                currentPID = -1;
                currentProc_SRTF = NULL;
            }
            time++;
            continue;
        }

        process *cur = extractMin(heap);
        if (cur == NULL) {
            time++;
            continue;
        }

        if (cur->remainingTime <= 0)
            continue;

        if (currentPID != -1 && currentPID != cur->id) {
            addGantt(currentPID, startTime, time);
            currentPID = cur->id;
            startTime = time;
        }
        else if (currentPID == -1) {
            currentPID = cur->id;
            startTime = time;
        }

        currentProc_SRTF = cur;

        if (preemptRequested) {
            preemptRequested = 0;
            insertHeap(heap, cur);
            currentPID = -1;
            currentProc_SRTF = NULL;
            continue;
        }

        cur->remainingTime--;
        time++;

        if (cur->remainingTime == 0) {
            cur->completionTime = time;
            cur->turnaroundTime = cur->completionTime - cur->arrivalTime;
            cur->waitingTime = cur->turnaroundTime - cur->burstTime;
            completed++;
            addGantt(cur->id, startTime, time);
            currentPID = -1;
            currentProc_SRTF = NULL;
        }
        else {
            insertHeap(heap, cur);
        }
    }

    printf("Completed SRTF Scheduling\n");
    freeHeap(heap);
}

void addGantt(int p_id, int start, int end) {
    if (ghead == NULL) {
        ghead = malloc(sizeof(gnode));
        ghead->p_id = p_id;
        ghead->start = start;
        ghead->end = end;
        ghead->link = NULL;
        return;
    }

    gnode *temp = ghead;
    while (temp->link != NULL)
        temp = temp->link;

    if (temp->p_id == p_id) {
        temp->end = end;
    } else {
        gnode *new = malloc(sizeof(gnode));
        new->p_id = p_id;
        new->start = start;
        new->end = end;
        new->link = NULL;
        temp->link = new;
    }
}

void viewGanttChart()
{
    if (ghead == NULL) {
        printf("No scheduling performed\n");
        return;
    }

    gnode *temp = ghead;

    while (temp != NULL) {
        printf("| P%d (%d -> %d) ", temp->p_id, temp->start, temp->end);
        temp = temp->link;
    }

    printf("|\n");
}

MinHeap* createHeap(int capacity) {
    MinHeap* h = malloc(sizeof(MinHeap));
    if (!h) return NULL;
    h->arr = malloc(sizeof(process*) * capacity);
    if (!h->arr) { free(h); return NULL; }
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void heapifyUp(MinHeap* h, int idx) {
    while (idx && h->arr[idx]->remainingTime < h->arr[(idx - 1) / 2]->remainingTime) {
        process *temp = h->arr[idx];
        h->arr[idx] = h->arr[(idx - 1) / 2];
        h->arr[(idx - 1) / 2] = temp;
        idx = (idx - 1) / 2;
    }
}

void heapifyDown(MinHeap* h, int idx) {
    int smallest = idx, left = 2 * idx + 1, right = 2 * idx + 2;
    if (left < h->size && h->arr[left]->remainingTime < h->arr[smallest]->remainingTime)
        smallest = left;
    if (right < h->size && h->arr[right]->remainingTime < h->arr[smallest]->remainingTime)
        smallest = right;
    if (smallest != idx) {
        process *temp = h->arr[idx];
        h->arr[idx] = h->arr[smallest];
        h->arr[smallest] = temp;
        heapifyDown(h, smallest);
    }
}

void insertHeap(MinHeap* h, process* p) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->arr = realloc(h->arr, h->capacity * sizeof(process*));
    }
    h->arr[h->size] = p;
    heapifyUp(h, h->size++);
}

process* extractMin(MinHeap* h) {
    if (h->size == 0) return NULL;
    process* min = h->arr[0];
    h->arr[0] = h->arr[--h->size];
    heapifyDown(h, 0);
    return min;
}

void freeHeap(MinHeap* h) {
    if (!h) return;
    free(h->arr);
    free(h);
}
