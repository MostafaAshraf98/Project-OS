#include "headers.h"

struct msgbuff
{
    long mtype;
    process* p;
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

void handler(int signum)
{
    printf("signal has been sent and scheduler is exiting\n");
    stillSending = false;
}

int main(int argc, char *argv[])
{
    printf("scheduler is executing\n");
    char buf1[500];
    getcwd(buf1, sizeof(buf1));
    const char *pathProcess = strcat(buf1, "/process.out");

    initClk();
    previousClk = getClk();

    process **arrSharedMemoryAdresses = (process **)malloc(sizeof(process *));
    int *arrPIDS = (int *)malloc(sizeof(int));
    int k = 0; // iterator on the array of Addresses

    signal(SIGUSR1, handler);

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
    process *p;  // received process
    process *p2; // running proces
    process *pointerToRunningProcess = NULL;
    int testinteger;

    while (stillSending)
    {
        currentClk = getClk();
        size_t sz = sizeof(struct msgbuff) - sizeof(long);
        rec_val = msgrcv(msgQ_id, &messageToReceive, sz, 0, IPC_NOWAIT); // receives messages with any mtype
        if (rec_val != -1)                                               // if the shcedule did receive a new process successfully
        {

            printf("received message\n");
            p = messageToReceive.p;
            switch (Algorithm)
            {
            case 1: // FCFS
                p->remainingTime = p->runTime;
                p->WaitingTime = 0;
                strcpy(p->state, "ready");
                p->priority = 0;
                break;
            case 2: //SJF
                p->remainingTime = p->runTime;
                p->WaitingTime = 0;
                strcpy(p->state, "ready");
                p->priority = p->runTime;
                break;
            case 3: //HPF
                p->remainingTime = p->runTime;
                p->WaitingTime = 0;
                strcpy(p->state, "ready");

                break;
            case 4: //SRTN
                p->remainingTime = p->runTime;
                p->WaitingTime = 0;
                strcpy(p->state, "ready");
                p->priority = p->remainingTime;
                break;
            case 5: //RR
                p->remainingTime = p->runTime;
                p->WaitingTime = 0;
                strcpy(p->state, "ready");
                p->priority = 0;
                break;
            }
            //here we want to make a IPC between process and scheduler
            key_t key_id;
            key_id = ftok("keyfile", secretNumber);
            shmid = shmget(key_id, sizeof(process), IPC_CREAT | 0644);   // create or verify the existence of a shared memory
            process *shmaddr = (process *)shmat(shmid, (process *)0, 0); // attach to shared memory address
            char buff[100];

            sprintf(buff, "%d", secretNumber);

            int process_pid = fork(); // forking the process
            if (process_pid == -1)
            {
                perror("error");
            }
            else if (process_pid == 0) //the process
            {
                execl(pathProcess, "process.out", buff, NULL); // the process
            }
            secretNumber++;
            enqueue(&pq, p);

            arrPIDS[k] = p->id;
            arrSharedMemoryAdresses[k] = shmaddr;

            arrSharedMemoryAdresses[k] = p;
            strcpy((arrSharedMemoryAdresses[k])->state, p->state);
            k++;
        }
        if (pointerToRunningProcess == NULL && !isEmpty(&pq)) // if there is not a running process
        {
            p2 = dequeue(&pq);
            pointerToRunningProcess = p2;
        }
        if ((currentClk - previousClk) == 1 && (pointerToRunningProcess != NULL))
        {
            incrementWaintingTime(&pq);
            pointerToRunningProcess->remainingTime -= 1;
            if (pointerToRunningProcess->remainingTime == 0)
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
