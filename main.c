#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

int main()
{
    int choice;
    while(1)
    {
        printf("\nCPU Scheduler\n");
        printf("\n1. Add Process\n2. FCFS\n3. RR\n4. SRTF\n5. View Gantt Chart\n6. Preempt Process\n7. View Ready Queue\n8. Exit\n");
        printf("Enter your choice\n");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                addProcess();
                printf("Process added successfully\n");
                break;

            case 2:
                resetAllProcesses();
                printf("First Come First Serve (FCFS) Scheduling Method\n");
                fcfsScheduling();
                break;

            case 3:
                resetAllProcesses();
                printf("Round Robin (RR) Scheduling Method\n");
                rrScheduling();
                break;

            case 4:
                resetAllProcesses();
                printf("Shortest Remaining Time First (SRTF) Scheduling Method\n");
                srtfScheduling();
                break;

            case 5:
                printf("Gantt Chart\n");
                viewGanttChart();
                break;

            case 6:
                preemptProcess();
                break;

            case 7:
                viewReadyQueue();
                break;

            case 8:
                exit(0);

            default:
                printf("Invalid choice\n");
        }
    }
}