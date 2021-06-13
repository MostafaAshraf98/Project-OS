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

//======================ARRAY OF RECEIVED PROCESSES (A COPY  OF THEM)=====================
void addProcessToArr(process *p1);
//Array of received process
process *arrProcesses;
int arrProSize;
//=================================================

void handler(int signum) // THe SIGUSER1 signal handler
{
    printf("signal has been sent and scheduler is exiting\n");
    stillSending = false;
}

int main(int argc, char *argv[])
{
    printf("scheduler is executing\n");

    //Setting the path for the processes
    char buf1[500];
    getcwd(buf1, sizeof(buf1));
    const char *pathProcess = strcat(buf1, "/process.out");

    //initializing the clock
    initClk();
    previousClk = getClk();

    //array of shared memory addresses with the processes in the scheduler to be able to communicate with them
    process **arrSharedMemoryAdresses = (process **)malloc(sizeof(process *));

    //array of process ids (not the ones of forking but the ones in the input file) to be able to identify which processes we are addressing to when we get the shared memory
    int *arrPIDS = (int *)malloc(sizeof(int));
    int k = 0; // iterator on the array of Addresses and the array of ids

    // //Array of received process
    // arrProcesses = NULL;
    // arrProSize = 0;

    signal(SIGUSR1, handler);

    //seeting the received arguments from the process generator ( which algorithm and the quantum value in case of RR)
    Algorithm = atoi(argv[1]);
    if (Algorithm == 5) // if RR
        Quantum = atoi(argv[2]);

    //Creating the message queue to IPC with the process generator
    key_t key_id;
    int msgQ_id;
    int rec_val;
    int secretNumber = 65;

    key_id = ftok("keyfile", 65);
    msgQ_id = msgget(key_id, 0666 | IPC_CREAT); // creates or verifies the existence of an up queue with this key_id

    //Setting the priority queue
    PriorityQueue *pq = newPriorityQueue();

    process *pointerToRunningProcess = NULL; // pointer to the running process

    //the condition that the scheduer continue scheduling the process
    //this condition only breaks if the process generator send the signal SIGUSR1 that it has sent all the processes
    //in addition to the other condition that there still a running process or processes in the queue
    while (stillSending)
    {
        currentClk = getClk(); // with each iteration i get the current clock

        size_t sz = sizeof(struct msgbuff) - sizeof(long);               // the size of the message sent on the message queue
        rec_val = msgrcv(msgQ_id, &messageToReceive, sz, 0, IPC_NOWAIT); // receives messages with any mtype
        process p = messageToReceive.p;
        strcpy(p.state, messageToReceive.p.state);
        if (rec_val != -1) // if the shcedule did receive a new process successfully
        {
            printf("received message\n");

            //here we want to make a IPC between process and scheduler (using shared memory)
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
                execl(pathProcess, "process.out", buff, NULL); // We pass the secret number to the process to attach to the same memory address
            }
            secretNumber++; // different secret number for the next process

            strcpy(shmaddr->state, "ready");
            shmaddr->arrivalTime = p.arrivalTime;
            shmaddr->id = p.id;
            shmaddr->WaitingTime = 0;
            shmaddr->runTime = p.runTime;
            shmaddr->remainingTime = p.runTime;

            switch (Algorithm)
            {
            case 1: // FCFS
                shmaddr->priority = 0;
                break;
            case 2: //SJF
                shmaddr->priority = shmaddr->runTime;
                break;
            case 3: //HptrRecProF
                break;
            case 4: //SRTN
                shmaddr->priority = shmaddr->remainingTime;
                break;
            case 5: //RR
                shmaddr->priority = 0;
                break;
            }
            //Adding the process to the queue
            enqueue(&pq, shmaddr); // the process is added to the ready queue

            arrPIDS[k] = shmaddr->id; // We add the id of the recevied process to the array of process ids

            //we need to put the address of the pocess in the shared memory address
            //so that this address points to the process
            arrSharedMemoryAdresses[k] = shmaddr;
            k++;
        }
        if (pointerToRunningProcess == NULL && !isEmpty(&pq)) // if there is not a running process
        {
            pointerToRunningProcess = dequeue(&pq);
        }
        if ((currentClk - previousClk) == 1 && (pointerToRunningProcess != NULL)) // There is a running a running process and a second has passed
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

// void addProcessToArr(process *p1)
// {
//     arrProSize++;
//     arrProcesses = (process *)realloc(arrProcesses, arrProSize * sizeof(process));
//     arrProSize--;
//     arrProcesses[arrProSize].id = p1->id;
//     arrProcesses[arrProSize].arrivalTime = p1->arrivalTime;
//     arrProcesses[arrProSize].priority = p1->priority;
//     arrProcesses[arrProSize].remainingTime = p1->remainingTime;
//     arrProcesses[arrProSize].runTime = p1->runTime;
//     arrProcesses[arrProSize].WaitingTime = p1->WaitingTime;
//     strcpy(arrProcesses[arrProSize].state, p1->state);
//     arrProSize++;
// }