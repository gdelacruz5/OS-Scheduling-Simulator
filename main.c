/* Gian Delacruz
 * CPE 453
 * Professor Peterson
 * Program 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parseFile.h"
#include "simulator.h"

//checks to see that the file can be open, also returns the file pointer
FILE* openFile(char *fileName){
   
   FILE *fp;

   if ((fp = fopen(fileName, "r")) == NULL)
   {
      perror(fileName);
      exit(EXIT_FAILURE);
   }

   return fp;
}

//checks the flag and mutates the appropriate variable
void checkFlag(int index, char *argv[], int *schedType, int *q){

   if (strcmp("-p", argv[index]) == 0)
   {
      if (strcmp("RR", argv[index + 1]) == 0)
      {
         *schedType = RR;
      }
      else if (strcmp("SRJN", argv[index + 1]) == 0)
      {
         *schedType = SRJN;
      }
   }
   else if (strcmp("-q", argv[index]) == 0)
   {
      sscanf(argv[index + 1], "%d", q);
   }
   else
   {
      fprintf(stderr, "Usage: schedSim <job-file.txt> -p <ALGORITHME> -q <QUANTUM>\n");
      exit(EXIT_FAILURE);
   }
}

//checks the arguments for the correct usage
FILE* checkArgs(int argc, char *argv[], int *schedType, int *q){

   if (argc == 6)
   {
      checkFlag(2, argv, schedType, q);
      checkFlag(4, argv, schedType, q);
   }
   else if (argc == 4)
   {
      checkFlag(2, argv, schedType, q);
   }
   else if (argc == 2)
   {
   }
   else
   {
      fprintf(stderr, "Usage: schedSim <job-file.txt> -p <ALGORITHME> -q <QUANTUM>\n");
      exit(EXIT_FAILURE);
   }

   return openFile(argv[1]);
}

int main(int argc, char *argv[])
{
   FILE *fp;
   int schedType = FIFO;
   int q = 1;

   fp = checkArgs(argc, argv, &schedType, &q);

   parseFile(fp, schedType, q);

   fclose(fp);

   return EXIT_SUCCESS;
}
