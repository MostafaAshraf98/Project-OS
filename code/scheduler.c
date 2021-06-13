#include "headers.h"

struct msgbuff
{
    int mtype;
    process p;
    char mtext[256];
};

typedef struct OutputLine
{
    int time;
    int process;
    int state;
    int arr;
    int total;
    int remain;
    int wait;
    int TA;  //turnarount time
    int WTA; // weighted turnaround time

} OutputLine;

bool stillSending = true;
int Algorithm;
int Quantum;
struct msgbuff messageToReceive;
int currentClk;
int previousClk;
const char *pathProcess = "/media/sf_Ubunto_Sharing_Folder/Project repo/Project-OS/code/process.out";

void handler(int signum)
{
    printf("signal has been sent and scheduler is exiting\n");
    stillSending = false;
}

int main(int argc, char *argv[])
{
    previousClk = getClk();
    int *arrAdresses = (int *)malloc(sizeof(int));
    int k = 0; // iterator on the array of Addresses
    printf("scheduler is executing\n");
    signal(SIGUSR1, handler);
    initClk();
    Algorithm = atoi(argv[1]);
    if (Algorithm == 5)
        Quantum = atoi(argv[2]);

    key_t key_id;
    int msgQ_id;
    int rec_val;
    int secretNumber = 65;

    key_id = ftok("keyfile", 65);
    msgQ_id = msgget(key_id, 0666 | IPC_CREAT); // creates or verifies the existence of an up queue with this key_id

    //TODO: implement the scheduler.
    PriorityQueue *pq = newPriorityQueue();
    process p;
    process *runningProcess = NULL;
    process p2;
    int testinteger;

    while (stillSending)
    {
        currentClk = getClk();
        rec_val = msgrcv(msgQ_id, &messageToReceive, sizeof(messageToReceive.p), 0, !IPC_NOWAIT); // receives messages with any mtype
        if (rec_val != -1)                                                                        // if the shcedule did receive a new process successfully
        {

            printf("received message\n");
            p = messageToReceive.p;
            switch (Algorithm)
            {
            case 1: // FCFS
                p.remainingTime = p.runTime;
                p.WaitingTime = 0;
                p.state = "ready";
                p.priority = 0;
                break;
            case 2: //SJF
                p.remainingTime = p.runTime;
                p.WaitingTime = 0;
                p.state = "ready";
                p.priority = p.runTime;
                break;
            case 3: //HPF
                p.remainingTime = p.runTime;
                p.WaitingTime = 0;
                p.state = "ready";
                break;
            case 4: //SRTN
                p.remainingTime = p.runTime;
                p.WaitingTime = 0;
                p.state = "ready";
                p.priority = p.remainingTime;
                break;
            case 5: //RR
                p.remainingTime = p.runTime;
                p.WaitingTime = 0;
                p.state = "ready";
                p.priority = 0;
                break;
            }
            // //here we want to make a IPC between process and scheduler

            // key_t key_id;
            // key_id = ftok("keyfile", secretNumber);
            // secretNumber++;
            // shmid = shmget(key_id, 4096, IPC_CREAT | 0644); // create or verify the existence of a shared memory
            // void *shmaddr = shmat(shmid, (void *)0, 0);     // attach to shared memory address
            // char buff[100];
            // sprintf(buff,"%d", secretNumber);
            // printf("The secret number in integer is: %d\n",secretNumber);
            // printf("The secret number from itoa is : %s\n",buff);
            // execl(pathProcess, "process.out", buff, NULL);

            enqueue(&pq, p);
        }
        if (runningProcess == NULL && !isEmpty(&pq)) // if there is not a running process
        {
            p2 = dequeue(&pq);
            runningProcess = &p2;
        }
        if ((currentClk - previousClk) == 1 && (runningProcess != NULL))
        {
            incrementWaintingTime(&pq);
            runningProcess->remainingTime -= 1;
            if (runningProcess->remainingTime == 0)
            {
            }
            switch (Algorithm) // based on algorithm we will decide what happens to the process in running
            {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            }

            previousClk = getClk();
        }
    }
    printf("scheduler exited the while loop\n");

    //TODO: upon termination release the clock resources.
    destroyClk(false);
    exit(0);
}
