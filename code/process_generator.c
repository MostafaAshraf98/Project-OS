#include "headers.h"
#include "PriorityQueue.h"
#include <time.h>
void clearResources(int);

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE *inputFile;
    char* inputFileName = argv[1];
    inputFile = fopen(inputFileName, "r"); //Opens file with parameters (path, reading only)
    //Parameters that will be readed from the input file
    int inputFileID;
    int inputFileArrTime;
    int inputFileRunTime;
    int inputFilePriority;

    int count = 0;
    char c;
    c = fgetc(inputFile);
    while (c != EOF)
    {
        if (c == '#')
        {
            char buff[255];
            fgets(buff, 255, inputFile);
        }
        else
        {
            count++;
        }
        c = fgetc(inputFile);
    }

    //inputFileId is the number of process
    process fileProcesses[count];
    int fileProcessesCount = 0;

    fclose(inputFile); //Closes the inputFile

    inputFile = fopen(inputFileName, "r"); //Opens file with parameters (path, reading only)

    c = fgetc(inputFile);
    while (c != EOF)
    {
        //With every loop, the values of inputFileID, inputFileArrTime, inputFileRunTime and inputFilePriority changes
        //The values are added to the array fileProcesses
        if (c == '#')
        {
            char buff[255];
            fgets(buff, 255, inputFile);
        }
        else
        {
            inputFileID = c - '0'; //To convert '0' --> 0, '1' --> 1 etc.
            fscanf(inputFile, "\t%d\t%d\t%d\n", &inputFileArrTime, &inputFileRunTime, &inputFilePriority);
            process p;
            p.id = inputFileID;
            p.arrivalTime = inputFileArrTime;
            p.runTime = inputFileRunTime;
            p.priority = inputFilePriority;
            fileProcesses[fileProcessesCount++] = p;
        }
        c = fgetc(inputFile);
    }

    //NOW we have an array of Processes "fileProcesses" with size "fileProcessesCount" that contains the parameters of each process
    /*
    for (int i = 0; i < fileProcessesCount; i++)
    {  
        printf("%d\t%d\t%d\t%d\n",fileProcesses[i].id,fileProcesses[i].arrivalTime,fileProcesses[i].runTime,fileProcesses[i].priority);
    }
    */
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    int Algorithm = atoi(argv[2]);
    if (Algorithm < 1 || Algorithm > 5)
    {
        printf("error \n");
    }
    int Quantum = 0;
    if (Algorithm == 5)
        Quantum = atoi(argv[3]);

    switch (Algorithm)
    {
    case 1:
        printf(" Scheduled Algorithm is First Come First Serve (FCFS)\n");
        break;
    case 2:
        printf("Scheduled Algorithm isShortest Job First (SJF)\n");
        break;
    case 3:
        printf("Scheduled Algorithm is Preemptive Highest Priority First (HPF) \n");
        break;
    case 4:
        printf("Scheduled Algorithm isShortest Remaining \n");
        break;
    case 5:
        printf("Scheduled Algorithm is Round Robin\n");
        break;
    }

    // 3. Initiate and create the scheduler and clock processes.

    int clk_Pid = fork();
    if (clk_Pid == -1)
        return -1;
    else if (clk_Pid == 0)
    {
        //clock
    }

    int scheduler_Pid = fork();
    if (scheduler_Pid == -1)
        return -1;
    else if (scheduler_Pid == 0)
    {
        //scheduler
    }

    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    // To get time use this function.
    int x = getClk();
    printf("Current Time is %d\n", x);
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption
}
