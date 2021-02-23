#ifndef SIMULATOR_H
#define SIMULATOR_H

#define FIFO 1
#define SRJN 2
#define RR   3

void simulateSched(Process procs[], int numProcs, int schedType, int q);

#endif
