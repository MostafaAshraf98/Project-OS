#include "headers.h"

/* Modify this file as needed*/
int remainingtime;

int main(int agrc, char *argv[])
{
    
    initClk();
    
    key_t key_id;
    int secretNumber= atoi(argv[1]);
    key_id = ftok("keyfile", secretNumber);
    shmid = shmget(key_id, 4096, IPC_CREAT | 0644); // create or verify the existence of a shared memory
    void *shmaddr = shmat(shmid, (void *)0, 0);     // attach to shared memory address
    
    
    //TODO The process needs to get the remaining time from somewhere
    //remainingtime = ??;
    while (remainingtime > 0)
    {
        // remainingtime = ??;
    }

    destroyClk(false);

    return 0;
}
