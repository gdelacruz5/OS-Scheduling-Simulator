#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseFile.h"
#include "simulator.h"

//sorts the procs by arrival time, also gives the job ID number
void sortProcs(Process procs[], int numProcs){
   
   int i;
   int j;
   Process temp;
  
   //sorts the processes by arrival time
   for(i = 0; i < numProcs; i++)
   {
      for(j = i + 1; j < numProcs; j++)
      {
         if (procs[i].arriveT > procs[j].arriveT)
         {
            temp = procs[i];
            procs[i] = procs[j];
            procs[j] = temp;
         }
      }
      (procs[i]).jobID = i;
   }

}

//parses the file
void parseFile(FILE *fp, int schedType, int q){

   Process *procs; //array to hold the processes
   char line[100]; //line buffer to get number of processes
   int numProcs = 0; //total number of processes
   int i;
   int check;

   while(fgets(line, 100, fp) != NULL)
   {
      numProcs++;
   }

   //sets the fp back to the beggining
   rewind(fp);

   //sends error message if the file is empty
   if (numProcs == 0)
   {
      fprintf(stderr, "empty file\n");
      exit(EXIT_FAILURE);
   }

   //mallocs the array of processes
   procs = malloc(numProcs * sizeof(Process));
   if (procs == NULL)
   {
      fprintf(stderr, "malloc failure\n");
      exit(EXIT_FAILURE);
   }

   //scans the procedures into the array
   for(i = 0; i < numProcs; i++)
   {
      check = fscanf(fp, "%f %f\n", &(procs[i]).runT, &(procs[i]).arriveT);
      if (check !=2)
      {
         fprintf(stderr, "Input text is not formatted properly\n");
         fprintf(stderr, "Usage: <burst time> <arrival time>\n");
         exit(EXIT_FAILURE);
      }

      (procs[i]).runTRemain = (procs[i]).runT;
      (procs[i]).bWaitT = (procs[i]).arriveT;
      (procs[i]).respT = 0;
      (procs[i]).waitT = 0;
   }

   //sorts the procs by arrival time
   sortProcs(procs, numProcs);

   //call to the simulator, gives it the procs and the total number of procs
   simulateSched(procs, numProcs, schedType, q);

   //frees the memory malloced to the processes
   free(procs);
}
