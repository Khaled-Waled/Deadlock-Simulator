#include <iostream>
using namespace std;

//Global variables and datastructures
int NUM_OF_RESOURCE_TYPES = 3;
int NUM_OF_Processes = 3;
int *available;
int **maximum;
int **allocation;
int **need;
//************************************


void greetingMessage()
{
    cout<<"*******************************\n";
    cout<<"    Initialising System...\n";
    cout<<"*******************************\n\n";
}

void initDataStructures()
{
    cout<<"*** Initializing resources...\n";
    cout<<"How many resources are in the system?\n";
    cin>>NUM_OF_RESOURCE_TYPES;

    available = new int[NUM_OF_RESOURCE_TYPES]; //Init available[]

    //Fill available[]
    cout<<"Please enter the amount of initially available resources:\n";
    for(int i=0; i<NUM_OF_RESOURCE_TYPES; i++)
    {
        printf("R%d:",i);
        int value=0;
        cin>>available[i];
    }



    cout<<"\n\n*** Initializing resources...\n";
    cout<<"How many processes are in the system?\n";
    cin>>NUM_OF_Processes;

    //init maximum[][]
    maximum = new int*[NUM_OF_Processes];
    for (int i=0; i<NUM_OF_Processes; i++)
        maximum[i] = new int[NUM_OF_RESOURCE_TYPES];

    //init allocation[][]
    allocation = new int*[NUM_OF_Processes];
    for (int i=0; i<NUM_OF_Processes; i++)
        allocation[i] = new int[NUM_OF_RESOURCE_TYPES];

    //init need[][]
    need = new int*[NUM_OF_Processes];
    for (int i=0; i<NUM_OF_Processes; i++)
        need[i] = new int[NUM_OF_RESOURCE_TYPES];


    //fill maximum[][] and allocation[][] and need[][]
    cout<<"For each process, enter resource info for each resource\n";
    cout<<"space separated like: R0 R1 R2...\n";
    for (int i=0; i<NUM_OF_Processes; i++)
    {
        //fill maximum[][]
        cout<<"Process #"<<i<<" has maximum resources needs of: \n";
        for (int j=0; i<NUM_OF_RESOURCE_TYPES; i++)
           cin>>maximum[i][j];

        //fill allocation[][]
        cout<<"\nProcess #"<<i<<" is already using resources of: \n";
        for (int j=0; i<NUM_OF_RESOURCE_TYPES; i++)
            cin>>allocation[i][j];

        //fill need[][]
        for (int j=0; i<NUM_OF_RESOURCE_TYPES; i++)
            need[i][j] = maximum[i][j] - allocation[i][j];
    }
}

bool isSafe(int pid, int request[NUM_OF_RESOURCE_TYPES])
{
    //add request in the right place and then check using banker algorithm
    //TODO @Islam
    return false;
}

int* getRequest()
{
    int pid=0;
    cin>>pid;
    int* request = new int[NUM_OF_RESOURCE_TYPES];
    //fill the request
    //TODO @Khaled
}

void releaseResources()
{
    int pid=0;
    cin>>pid;

    //deallocate and fix other matrices
    //TODO @Khaled
}

//DON'T RUN until fixed!
void recover()
{
    if (isSafe) return;
    else recover();
    //TODO @Hagry
}

int main()
{
    greetingMessage();
    initDataStructures();

    string command="";
    while (cin>>command)
    {
        if (command == "RQ")
            getRequest();

        else if(command == "RL")
            releaseResources();
        else
            return (0);
    }

}
