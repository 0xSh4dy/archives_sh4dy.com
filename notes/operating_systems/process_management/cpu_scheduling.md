---
layout: page
title: CPU Scheduling
css: "assets/css/modeOne.css"
tags : [os,cs_concepts]
---

<hr>

### Introduction

CPU Scheduling is the basis of multiprogramming operating systems. In single processor systems, only one process can run at a time. All others must wait until the CPU becomes free and can be rescheduled. A process is executed until it is instructed to wait, typically for the completion of some I/O request. Then the CPU sits idle and all this waiting time is wasted and no useful work is accomplished. The goal of CPU scheduling is to utilize this time. A scheduling system allows one process to use the CPU while another is waiting for I/O, thereby making full use of otherwise lost CPU cycles.

### CPU and I/O Burst Cycles
Process execution consists of a cycle of CPU execution and I/O wait. CPU Burst is the time when a process is under CPU execution. I/O burst is the time when a process is waiting for I/O. Process execution starts with a CPU burst followed by an I/O burst followed by a CPU burst followed by an I/O burst and so on. The final CPU burst ends with a system request to terminate execution.

### Scheduling Criteria
The efficiency of a scheduling algorithm depends on the following:

1. `CPU Utilization`: The CPU must be as busy as possible to ensure efficient utilization.

2. `Throughput`: If the CPU is busy executing processes, then work is being done. Throughput is the number of processes completed per unit time. It is a measure of work done.

3. `Turnaround time`: The amount of time it takes to complete a process or fulfill a request. It is the interval from the time of submission of a process to the time of completion.
```
Turnaround time = Completion time - arrival time
```

4. `Waiting time`: The CPU scheduling algorithm does not affect the amount of time during which a process executes or does I/O, it only affects the time spent by a process, waiting in the ready queue. Waiting time is the sum of periods spend waiting in the ready queue.
```
Waiting time = total waiting time - amount of time the process executed before waiting - arrival time

Waiting time = turn around time - burst time
```

5. `Response time`: A process can produce some output fairly early and can continue computing new results while previous results are being output to the user. Response time is the time from the submission of a process until the first response is produced.

<hr>

### Scheduling Algorithms

#### First-Come, First-Served Scheduling
The process that requests the CPU first is allocated the CPU first. The implementation of FCFS policy is implemented using a FIFO queue. The average waiting time under the FCFS policy may vary, it depends on order in which the processes are added to the queue. Sometimes, process with low burst time may have to wait for the completion of processes with high burst time. The FCFS scheduling algorithm is non-preemptive. Once the CPU has been allocated to a process, that process keeps the CPU until it releases it,either by IO burst or termination. This algorithm is troublesome for time-sharing systems, where multiple people should access the same system simultaneously, 

#### Shortest-Job-First Scheduling
When the CPU is available, it is assigned to the process that has the smallest next CPU burst. If the next CPU burst of two processes are the same, FCFS scheduling is used to break the tie. SJF algorithm can be either preemptive or non-preemptive. Preemptive SJF scheduling is sometimes known as Shortest-remaining-time first scheduling. Calculating the next CPU burst time is very difficult, one of the shortcomings of SJF algorithm.

#### Priority Scheduling
In priority scheduling, a priority is associated with each process, and the process with the highest priority is allocated the CPU first. Process with equal priority are scheduled according to FCFS scheduling algorithm. Priority scheduling can either be preemptive or non-preemptive. A preemptive priority scheduling algorithm will preempt the CPU if the priority of the newly arrived process is higher than the priority of the currently running process. A non-preemptive priority scheduling algorithm will simply put the process with high priority at the head of the ready queue.
A process that is ready to run but waiting for the CPU can be considered blocked. A major problem with priority scheduling is indefinite blocking, or starvation. In some cases, process with low priority may have to wait indefinitely, causing starvation (the process is starving to get the CPU). A solution to the problem of starvation is `aging`. Aging is a technique of gradually increasing the priority of processes that wait in the system for a long time.

#### Round Robin Scheduling
The RR algorithm is specially designed for time-sharing systems. It is similar to FCFS scheduling, but preemption is used to switch between processes. It defines a small unit of time known as `time quantum` or `time slice` (usually ranges from 10 to 100 milliseconds). A process is allowed to execute only for the specified time quantum after which the CPU is given to the next process in the ready queue for the specified time quantum. In this case, the ready queue is treated as a circular queue. The CPU scheduler goes around the ready queue, allocating the CPU to each process for a time interval of upto 1 time quantum. Time quantum should be same for all the processes in the queue. The ready queue is a circular FIFO queue, new processes are added to the tail of the queue. The CPU scheduler picks the first process from the ready queue, sets a timer to interrupt after one time quantum and dispatches the process. If the burst time is less than 1 time quantum, the process will release the CPU voluntarily. The CPU scheduler will then select the next process in the ready queue. If the CPU burst is greater than 1 time quantum, the timer will go off at 1 time quantum and cause an interrupt. After that, a context switch will occur and the process will be placed at the tail of the ready queue.

```
Waiting time = last start time - arrival time - (preemption * time quantum (in ms))

Preemption = no. of times a process is preempted.
```

#### Multilevel Queue Scheduling
It partitions the ready queue into several separate queues. Processes are assigned to a queue, based on some property of that process, such as memory size, process priority or process type. Each queue has its own scheduling algorithm. In addition, there must be scheduling among the queues, which is generally implemented using FCFS scheduling.

#### Multilevel Feedback Queue Scheduling
This algorithm allows a process to move between queues. Processes are separated according to the characteristics of their CPU bursts. If a process uses too much time, it is moved to a lower priority queue. I/O bound and interactive processes are moved to higher priority queues. In addition, a process that waits for too long in a lower priority queue may be moved to a higher priority queue. This form of aging prevents starvation. A multilevel feedback queue scheduler is defined by the following parameters:
- The number of queues.
- The scheduling algorithm for each queue.
- The method used to determine when to upgrade a process to a higher priority queue.
- The method used to determine when to demote a process to a lower priority queue.
- The method used to determine which queue a process will enter when that process needs service.

<hr>