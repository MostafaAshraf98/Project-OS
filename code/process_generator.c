#include "headers.h"
#include "PriorityQueue.h"

void clearResources(int);

struct Processes
{
    int ID;
    int arrivalTime;
    int runningTime;
    int priority;
};

typedef struct Processes Processes;     //to declare Process without writing struct

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    // 1. Read the input files.
    FILE* inputFile;
    inputFile = fopen("processes.txt", "r");    //Opens file with parameters (path, reading only)
    //Parameters that will be readed from the input file
    int inputFileID;
    int inputFileArrTime;
    int inputFileRunTime;
    int inputFilePriority;

    
    char buff[255];
    fgets(buff,255,inputFile);  //This reads the first line "#id arrival runtime priority" which is not important
    
    while ( fscanf( inputFile, "%d\t%d\t%d\t%d\n", &inputFileID, &inputFileArrTime, &inputFileRunTime, &inputFilePriority) != EOF )
    {
        //This while loop is used to get the number of processes in the file to create an array
    }

    //inputFileId is the number of process
    Processes fileProcesses[inputFileID];
    int fileProcessesCount = 0;

    fclose(inputFile);  //Closes the inputFile

    

    inputFile = fopen("processes.txt", "r");    //Opens file with parameters (path, reading only)

    inputFile = fopen("processes.txt", "r");    //Opens file with parameters (path, reading only)
    fgets(buff,255,inputFile);  //This reads the first line "#id arrival runtime priority" which is not important
    while ( fscanf( inputFile, "%d\t%d\t%d\t%d\n", &inputFileID, &inputFileArrTime, &inputFileRunTime, &inputFilePriority) != EOF )
    {
        //With every loop, the values of inputFileID, inputFileArrTime, inputFileRunTime and inputFilePriority changes
        //The values are added to the array fileProcesses
        Processes p;
        p.ID            = inputFileID;
        p.arrivalTime   = inputFileArrTime;
        p.runningTime   = inputFileRunTime;
        p.priority      = inputFilePriority;
        fileProcesses[fileProcessesCount++] = p;
    }
    
    //NOW we have an array of Processes "fileProcesses" with size "fileProcessesCount" that contains the parameters of each process
    

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
    case 1:  printf(" Scheduled Algorithm is First Come First Serve (FCFS)\n");
         break;
    case 2:  printf("Scheduled Algorithm isShortest Job First (SJF)\n");
        break;
    case 3: printf("Scheduled Algorithm is Preemptive Highest Priority First (HPF) \n");
        break;
    case 4: printf("Scheduled Algorithm isShortest Remaining \n");
        break;
    case 5:  printf("Scheduled Algorithm is Round Robin\n");
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
