#include <stdio.h>
#include <stdlib.h>
// priority Node

typedef struct process
{
   char *state;
   int id;
   int arrivalTime;
   int priority;
   int runTime;
   int executionTime;
   int remainingTime;
   int WaitingTime;
} process;

void printProcess(process p)
{
   printf("id is: %d\n", p.id);
   printf("prioriry is: %d\n", p.priority);
   printf("state is: %s\n", p.state);
   printf("arrival times is: %d\n", p.arrivalTime);
   printf("runTime is: %d\n", p.runTime);
   printf("execution time is: %d\n", p.executionTime);
   printf("remaining time is: %d\n", p.remainingTime);
   printf("Waiting time is: %d\n\n", p.WaitingTime);
}

typedef struct node
{
   process p;
   struct node *next;
} Node;

Node *newNode(process p)
{
   Node *temp = (Node *)malloc(sizeof(Node));
   temp->p = p;
   temp->next = NULL;
   return temp;
}

// Function to check the queue is empty
int isEmpty(Node **head)
{
   return (*head) == NULL;
}

process front(Node **head)
{
   if (isEmpty(head))
   {
      process p;
      return p;
   }
   return (*head)->p;
}

process dequeue(Node **head)
{
   if (isEmpty(head))
   {
      process p;
      return p;
   }
   Node *temp = *head;
   (*head) = (*head)->next;
   temp->p.state = "running";
   process returnedProcess = temp->p;
   free(temp);
   return returnedProcess;
}

void enqueue(Node **head, process p)
{
   if (isEmpty(head))
      return;
   Node *start = (*head);
   Node *temp = newNode(p);
   if ((*head)->p.priority > p.priority)
   {
      temp->next = *head;
      (*head) = temp;
   }
   else
   {
      while (start->next != NULL &&
             start->next->p.priority <= p.priority)
      {
         start = start->next;
      }
      // Either at the ends of the list
      // or at required position
      temp->next = start->next;
      start->next = temp;
   }
}

void incrementWaintingTime(Node **head)
{
   if (isEmpty(head))
      return;
   Node *start = (*head);
   start->p.WaitingTime++;
   while (start->next != NULL)
   {
      start->next->p.WaitingTime++;
      start = start->next;
   }
}

void printQueue(Node **head)
{
   if (isEmpty(head))
      return;
   Node *start = (*head);
   printProcess(start->p);
   while (start->next != NULL)
   {
      printProcess(start->next->p);
      start = start->next;
   }
}

// int main()
// {

//    process p1;
//    p1.state = "running";
//    p1.id = 1;
//    p1.priority=1;
//    p1.arrivalTime = 1;
//    p1.executionTime = 2;
//    p1.remainingTime = 3;
//    p1.runTime = 4;
//    p1.WaitingTime = 6;

//    process p2;
//    p2.state = "ready";
//    p2.priority=2;
//    p2.id = 2;
//    p2.arrivalTime = 10;
//    p2.executionTime = 21;
//    p2.remainingTime = 32;
//    p2.runTime = 14;
//    p2.WaitingTime = 64;

//    process p3;
//    p3.state = "ready";
//    p3.priority=0;
//    p3.id = 3;
//    p3.arrivalTime = 11;
//    p3.executionTime = 24;
//    p3.remainingTime = 31;
//    p3.runTime = 12;
//    p3.WaitingTime = 64;

//    Node *pq = newNode(p1); // creating the priority queue
//    enqueue(&pq, p2);       // creates inside it the new node and links it to the first node
//    enqueue(&pq, p3);
//    printQueue(&pq);
//    incrementWaintingTime(&pq);
//    printQueue(&pq);
//    // printProcess(dequeue(&pq));

//    return 0;
// }