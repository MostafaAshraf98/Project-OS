#include "headers.h"

/* Modify this file as needed*/
int remainingtime;
int currentClk;
int previousClk;

int main(int agrc, char *argv[])
{
    printf("process is running\n");

    initClk();
    previousClk = getClk();
    key_t key_id;
    int secretNumber = atoi(argv[1]);
    key_id = ftok("keyfile", secretNumber);
    shmid = shmget(key_id, sizeof(process), IPC_CREAT | 0644);   // create or verify the existence of a shared memory
    process *shmaddr = (process *)shmat(shmid, (process *)0, 0); // attach to shared memory address

    // printProcess(shmaddr);

    //TODO The process needs to get the remaining time from somewhere
    remainingtime = shmaddr->remainingTime;

    while (remainingtime > 0)
    {
        currentClk = getClk();
        if ((currentClk - previousClk) >= 1 && (strcmp(shmaddr->state, "started") == 0 || strcmp(shmaddr->state, "resumed") == 0))
        {
            remainingtime--;
            shmaddr->remainingTime -= 1;
            previousClk = getClk();
        }
    }
    shmaddr->id=shmid; // this is done to share the id with the scheduler
    destroyClk(false);
    exit(0);
    return 0;
}
