#include <stdio.h>
#include <stdlib.h>
#include "parseFile.h"
#include "simulator.h"
#include "linkedList.h"

float gTime = 0;//global time for the simulator
float tResponse = 0; //total response time of the sim
float tWait = 0; //total wait time of the sim
float tTurnaround = 0; //total turnaround time of the sim

//queues arrivals
void queueNextA(Process procs[], int numProcs, ListNode *list, int *next){

   while(*next < numProcs)
   {
      if (gTime >= (procs[*next]).arriveT)
      {
         addTail(list, *next);
         *next = *next + 1;
      }
      else
      {
         break;
      }
   }
}

//simulates round robin scheduling
void simRR(Process procs[], int numProcs, int q){

   ListNode *head;
   ListNode *temp;
   int nextA = 1;
   int pRemain = numProcs;
   int i;

   head = malloc(sizeof(ListNode));
   if (head == NULL)
   {
      fprintf(stderr, "malloc failure\n");
      exit(EXIT_FAILURE);
   }

   head->value = 0;
   head->next = NULL;

   if (gTime < (procs[head->value]).arriveT)
   {
      gTime = (procs[head->value]).arriveT;
   }

   queueNextA(procs, numProcs, head, &nextA);

   while(pRemain > 0)
   {
      //gets the resonse time
      if ((procs[head->value]).runTRemain ==
            (procs[head->value]).runT)
      {
         (procs[head->value]).respT = gTime - (procs[head->value]).arriveT + 1;
      }
      
      //adds to the wait time
      (procs[head->value]).waitT = (procs[head->value]).waitT +
         (gTime - (procs[head->value]).bWaitT);

      if (q >= (procs[head->value]).runTRemain)
      {
         //queue anything that arrives while the process is running
         gTime = gTime + (procs[head->value]).runTRemain - 1;
         queueNextA(procs, numProcs, head, &nextA);

         //remove from queue
         gTime = gTime + 1;
         (procs[head->value]).runTRemain = 0;
         (procs[head->value]).turnT = gTime - (procs[head->value]).arriveT;
         head = deleteNode(head, 0);
         pRemain--;

         //in case a proc finishes before the next one arrives
         if ((head == NULL) && (nextA < numProcs))
         {
            head = malloc(sizeof(ListNode));
            head->value = nextA;
            head->next = NULL;
            nextA++;
            if (gTime < (procs[head->value]).arriveT)
            {
               gTime = (procs[head->value]).arriveT;
            }
         }
      }
      else
      {
         //queue anything that arrives while the process is running
         gTime = gTime + q - 1;
         queueNextA(procs, numProcs, head, &nextA);

         //push this to the back of the queue
         gTime = gTime + 1;
         (procs[head->value]).runTRemain = (procs[head->value]).runTRemain - q;
         (procs[head->value]).bWaitT = gTime;
         
         if (head->next != NULL)
         {
            temp = head;
            head = head->next;
            temp->next = NULL;
            addTail(head, temp->value);
         }
      }
      
      //queue anything that has arrived as the process has had its turn
      queueNextA(procs, numProcs, head, &nextA);
   }

   //printing the information
   for(i = 0; i < numProcs; i++)
   {
      printf("Job %3d -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n",
         (procs[i]).jobID, (procs[i]).respT, (procs[i]).turnT, (procs[i]).waitT);
      tResponse = tResponse + (procs[i]).respT;
      tTurnaround = tTurnaround + (procs[i]).turnT;
      tWait = tWait + (procs[i]).waitT;
   }

   printf("Average -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n", 
   (tResponse / numProcs) , (tTurnaround / numProcs), (tWait / numProcs));
}

//sorts items in the queue by their run time remaining
void sortByRemainingTime(int queue[], Process procs[], int startOfQ,
   int nextOpen){

   int i;
   int j;
   int temp;

   for(i = startOfQ; i < nextOpen - 1; i++)
   {
      for(j = i + 1; j < nextOpen; j++)
      {
         if ((procs[queue[i]]).runTRemain > (procs[queue[j]]).runTRemain)
         {
            temp = queue[i];
            queue[i] = queue[j];
            queue[j] = temp;
         }
      }
   }
}

//queues jobs that have arrived
void queueNext(Process procs[], int *nextOpen, int numProcs){

   //int i; //for testing

   while(*nextOpen < numProcs)
   {
      if (gTime == (procs[*nextOpen]).arriveT)
      {
         *nextOpen = *nextOpen + 1;
      }
      else
      {
         break;
      }
   }
}

//simulates SRJN scheduling
void simSRJN(Process procs[], int numProcs){

   int *queue;
   int startOfQ = 0;
   int nextOpen = 0;
   int i;
   
   //malloc the queue and error check
   queue = malloc(numProcs * sizeof(int));
   if (queue == NULL)
   {
      fprintf(stderr, "malloc failure\n");
      exit(EXIT_FAILURE);
   }

   //adds everything to the queue, not in SRJN order
   for(i = 0; i < numProcs; i++)
   {
      queue[i] = i;
   }

   //if the first arrival is greater than 0 then increase time to arriveT
   if (gTime < (procs[queue[startOfQ]]).arriveT)
   {
      gTime = (procs[queue[startOfQ]]).arriveT;
   }

   //Add everything else to the queue that has arrived
   queueNext(procs, &nextOpen, numProcs);

   //sort by remaining time
   sortByRemainingTime(queue, procs, startOfQ, nextOpen);

   //run through processes and continue adding to the queue
   while(nextOpen < numProcs)
   {
      //gets the resonse time
      if ((procs[queue[startOfQ]]).runTRemain ==
            (procs[queue[startOfQ]]).runT)
      {
         (procs[queue[startOfQ]]).respT = gTime - (procs[queue[startOfQ]]).arriveT + 1;
      }

      //adds to the wait time
      (procs[queue[startOfQ]]).waitT = (procs[queue[startOfQ]]).waitT +
         (gTime - (procs[queue[startOfQ]]).bWaitT);

      if ((gTime + (procs[queue[startOfQ]]).runTRemain) == 
            (procs[nextOpen]).arriveT)
      {
         gTime = gTime + (procs[queue[startOfQ]]).runTRemain;
         (procs[queue[startOfQ]]).runTRemain = 0;
         (procs[queue[startOfQ]]).turnT = 
            gTime - (procs[queue[startOfQ]]).arriveT;
         startOfQ++;
         queueNext(procs, &nextOpen, numProcs);
         sortByRemainingTime(queue, procs, startOfQ, nextOpen);
      }
      else if ((gTime + (procs[queue[startOfQ]]).runTRemain) < 
            (procs[nextOpen]).arriveT)
      {
         gTime = gTime + (procs[queue[startOfQ]]).runTRemain;
         (procs[queue[startOfQ]]).runTRemain = 0;
         (procs[queue[startOfQ]]).turnT = 
            gTime - (procs[queue[startOfQ]]).arriveT;
         startOfQ++;
      }
      else
      {
         (procs[queue[startOfQ]]).runTRemain = 
            (procs[queue[startOfQ]]).runTRemain - 
               ((procs[nextOpen]).arriveT - gTime);
         gTime = (procs[nextOpen]).arriveT;
         (procs[queue[startOfQ]]).bWaitT = gTime; 
         queueNext(procs, &nextOpen, numProcs);
         sortByRemainingTime(queue, procs, startOfQ, nextOpen);
      }
   }

   //all items are added to the queue and sorted by remaining time, just run
   for(i = startOfQ; i < numProcs; i++)
   {
      //gets the response time
      if ((procs[queue[startOfQ]]).runTRemain ==
            (procs[queue[startOfQ]]).runT)
      {
         (procs[queue[startOfQ]]).respT = gTime - (procs[queue[startOfQ]]).arriveT + 1;
      }
   
      //adds to the wait time
      (procs[queue[startOfQ]]).waitT = (procs[queue[startOfQ]]).waitT +
         (gTime - (procs[queue[startOfQ]]).bWaitT);

      gTime = gTime + (procs[queue[startOfQ]]).runTRemain;
      (procs[queue[startOfQ]]).runTRemain = 0;
      (procs[queue[startOfQ]]).turnT =
         gTime - (procs[queue[startOfQ]]).arriveT;
      startOfQ++;
   }

   //printing the information
   for(i = 0; i < numProcs; i++)
   {
      printf("Job %3d -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n",
         (procs[i]).jobID, (procs[i]).respT, (procs[i]).turnT, (procs[i]).waitT);
      tResponse = tResponse + (procs[i]).respT;
      tTurnaround = tTurnaround + (procs[i]).turnT;
      tWait = tWait + (procs[i]).waitT;
   }

   printf("Average -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n", 
   (tResponse / numProcs) , (tTurnaround / numProcs), (tWait / numProcs));

   //free the queue memory
   free(queue);
}

//simulates FCFS scheduling
void simFCFS(Process procs[], int numProcs){

   int i;

   //completes each proc in the order it came
   for(i = 0; i < numProcs; i++)
   {
      if (gTime < (procs[i]).arriveT)
      {
         gTime = (procs[i]).arriveT;
      }
      (procs[i]).waitT = gTime - (procs[i]).arriveT; 
      (procs[i]).respT = (procs[i]).waitT + 1;
      (procs[i]).turnT = (procs[i]).waitT + (procs[i]).runT;

      gTime = gTime + (procs[i]).runT;
      tResponse = tResponse + (procs[i]).respT;
      tWait = tWait + (procs[i]).waitT;
      tTurnaround = tTurnaround + (procs[i]).turnT;

      printf("Job %3d -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n",
         (procs[i]).jobID, (procs[i]).respT, (procs[i]).turnT, (procs[i]).waitT);
   }
   
   printf("Average -- Response: %3.2f  Turnaround %3.2f  Wait %3.2f\n",
      (tResponse / numProcs), (tTurnaround / numProcs), (tWait / numProcs));
}

//simulates a scheduler for the specific type given
void simulateSched(Process procs[], int numProcs, int schedType, int q){

   if (schedType == RR)
   {
      simRR(procs, numProcs, q);
   }
   else if (schedType == SRJN)
   {
      simSRJN(procs, numProcs);
   }
   else
   {
      simFCFS(procs, numProcs);
   }
}
