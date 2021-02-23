#ifndef PARSEFILE_H
#define PARSEFILE_H

typedef struct{
   int jobID;
   float runT;
   float runTRemain;
   float arriveT;
   float respT;
   float waitT;
   float bWaitT; //beginning wait time
   float turnT;
} Process;

void parseFile(FILE *fp, int schedType, int q);

#endif
