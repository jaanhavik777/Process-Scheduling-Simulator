# CPU Scheduling Simulator

A **menu-driven CPU scheduling simulator** implemented in **C**, demonstrating classic CPU scheduling algorithms used in Operating Systems.
The program allows users to add processes, apply different scheduling algorithms, view the ready queue, manually preempt processes, and display a Gantt chart.

------------------------------

##  Features

* Add multiple processes with:

  * Process ID
  * Arrival Time
  * Burst Time

* Supports multiple scheduling algorithms:

  * **FCFS (First Come First Serve)**
  * **Round Robin (RR)** with configurable time quantum
  * **SRTF (Shortest Remaining Time First)** (preemptive)

* **Manual preemption** support (RR & SRTF)

* Displays:

  * **Ready Queue**
  * **Gantt Chart**

* Calculates:

  * Completion Time
  * Turnaround Time
  * Waiting Time

------------------------------

## Project Structure

```
.
├── main.c          # Menu-driven user interface
├── scheduler.c    # Core scheduling logic and algorithms
├── scheduler.h    # Data structures and function declarations
```

------------------------------

## Scheduling Algorithms Implemented

### 1. FCFS (First Come First Serve)

* Processes are executed in order of arrival.
* Non-preemptive scheduling.
* Simple queue-based implementation.

### 2. Round Robin (RR)

* Preemptive scheduling with a time quantum.
* Uses a separate ready queue.
* Supports manual preemption to rotate the current process.

### 3. SRTF (Shortest Remaining Time First)

* Preemptive version of SJF.
* Uses a **Min Heap** based on remaining burst time.
* Automatically preempts when a shorter job arrives.
* Manual preemption is also supported.

------------------------------

## Data Structures Used

* **Linked List Queue** – Ready queues and process lists
* **Min Heap** – Efficient selection of shortest remaining time (SRTF)
* **Linked List (Gantt Nodes)** – Gantt chart representation

------------------------------
## How to Compile and Run

### Compile

```bash
gcc main.c scheduler.c -o scheduler
```

### Run

```bash
./scheduler
```

------------------------------

## Menu Options

```
1. Add Process
2. FCFS Scheduling
3. Round Robin Scheduling
4. SRTF Scheduling
5. View Gantt Chart
6. Preempt Process
7. View Ready Queue
8. Exit
```

------------------------------

## Gantt Chart Output

The Gantt chart is printed in the format:

```
| P1 (0 -> 3) | P2 (3 -> 7) | P1 (7 -> 10) |
```

This clearly shows which process ran and during what time interval.

------------------------------

## Learning Objectives

This project helps understand:

* CPU scheduling concepts
* Preemptive vs non-preemptive scheduling
* Ready queue management
* Heap-based scheduling
* Real-time simulation of scheduling decisions

