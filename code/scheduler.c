#include "headers.h"

struct msgbuff
{
    long mtype;
    process p;
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

//=============================================
void addProcessToArr(process *p1);
//Array of received process
process *arrProcesses;
int arrProSize;
//=================================================
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

    //======================================================================
    //Array of received process
    arrProcesses = NULL;
    arrProSize = 0;
    //=====================================================================

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
    process *ptrRecPro; // received process
    process *p2;        // running proces
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
            addProcessToArr(&(messageToReceive.p));
            
            arrProSize--;

            printf("%d\t%d\t%d\t%d\n",arrProcesses[arrProSize].id,arrProcesses[arrProSize].arrivalTime
            ,arrProcesses[arrProSize].runTime,arrProcesses[arrProSize].priority);
            
            ptrRecPro = & arrProcesses[arrProSize];
            arrProSize++;
            
            switch (Algorithm)
            {
            case 1: // FCFS
                ptrRecPro->remainingTime = ptrRecPro->runTime;
                ptrRecPro->WaitingTime = 0;
                strcpy(ptrRecPro->state, "ready");
                ptrRecPro->priority = 0;
                break;
            case 2: //SJF
                ptrRecPro->remainingTime = ptrRecPro->runTime;
                ptrRecPro->WaitingTime = 0;
                strcpy(ptrRecPro->state, "ready");
                ptrRecPro->priority = ptrRecPro->runTime;
                break;
            case 3: //HptrRecProF
                ptrRecPro->remainingTime = ptrRecPro->runTime;
                ptrRecPro->WaitingTime = 0;
                strcpy(ptrRecPro->state, "ready");

                break;
            case 4: //SRTN
                ptrRecPro->remainingTime = ptrRecPro->runTime;
                ptrRecPro->WaitingTime = 0;
                strcpy(ptrRecPro->state, "ready");
                ptrRecPro->priority = ptrRecPro->remainingTime;
                break;
            case 5: //RR
                ptrRecPro->remainingTime = ptrRecPro->runTime;
                ptrRecPro->WaitingTime = 0;
                strcpy(ptrRecPro->state, "ready");
                ptrRecPro->priority = 0;
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

            //Adding the process to the queue
            enqueue(&pq, ptrRecPro);
            // process* frot = front(&pq);
            
            // printf("%d\t%d\t%d\t%d\n",arrProcesses[arrProSize].id,arrProcesses[arrProSize].arrivalTime
            // ,arrProcesses[arrProSize].runTime,arrProcesses[arrProSize].priority);
            // printProcess(frot);
            // fort->runTime--;
            // printf("==========================after\n");
            // printProcess(frot);

            arrPIDS[k] = ptrRecPro->id;
            arrSharedMemoryAdresses[k] = shmaddr;

            arrSharedMemoryAdresses[k] = ptrRecPro;
            strcpy((arrSharedMemoryAdresses[k])->state, ptrRecPro->state);
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

void addProcessToArr(process *p1)
{
    arrProSize++;
    arrProcesses = (process *)realloc(arrProcesses, arrProSize*sizeof(process));
    arrProSize--;
    arrProcesses[arrProSize].id             = p1->id;
    arrProcesses[arrProSize].arrivalTime    = p1->arrivalTime;
    arrProcesses[arrProSize].priority       = p1->priority;
    arrProcesses[arrProSize].remainingTime  = p1->remainingTime;
    arrProcesses[arrProSize].runTime        = p1->runTime;
    arrProcesses[arrProSize].WaitingTime    = p1->WaitingTime;
    strcpy(arrProcesses[arrProSize].state,   p1->state);
    arrProSize++;
}