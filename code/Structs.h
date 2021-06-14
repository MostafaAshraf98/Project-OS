#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct process
{
   char state[20];
   int id;
   int arrivalTime;
   int priority;
   int runTime;
   int remainingTime;
   int WaitingTime;
   int memsize;
   int memStartAddr;
   int memEndAddr;
} process;

typedef struct node
{
   process* p;
   struct node *next;
} Node;