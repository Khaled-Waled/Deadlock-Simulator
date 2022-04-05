#include <iostream>
#include <vector>
using namespace std;

//Global variables and data structures
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
    cout<<"*** Initializing resources ***\n";
    cout<<"How many resources are in the system?\n";
    cin>>NUM_OF_RESOURCE_TYPES;

    available = new int[NUM_OF_RESOURCE_TYPES]; //Init available[]

    //Fill available[]
    cout<<"Please enter the amount of initially available resources:\n";
    for(int i=0; i<NUM_OF_RESOURCE_TYPES; i++)
    {
        printf("R%d:",i);
        cin>>available[i];
    }


    cout<<"\n\n*** Initializing resources ***\n";
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
        for (int j=0; j<NUM_OF_RESOURCE_TYPES; j++)
           cin>>maximum[i][j];

        //fill allocation[][]
        cout<<"\nProcess #"<<i<<" is already using resources of: \n";
        for (int j=0; j<NUM_OF_RESOURCE_TYPES; j++)
            cin>>allocation[i][j];

        //fill need[][]
        for (int j=0; j<NUM_OF_RESOURCE_TYPES; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
    }
}

void printMatrix(int** mat)
{
    for (int i=0; i<NUM_OF_Processes; i++)
    {
        cout<<"Row "<<i<<':';
        for (int j=0; j<NUM_OF_RESOURCE_TYPES; j++)
            cout<<mat[i][j]<<" ";
    }
    cout<<'\n';
}



//copy sz blocks from src to dest
void cpy(int* src, int* dest, int sz)
{
    for(int i=0;i<sz;i++)
    {
        dest[i] = src[i];
    }
}

//cpy for 2d
void cpy2d(int** src, int** dest, int n, int m)
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            dest[i][j] = src[i][j];
        }
    }
}

//allocate memory 1d
int* allocate1d(int n)
{
    return new int[n];
}

//allocate memory 2d
int** allocate2d(int n, int m)
{
    int** arr = new int*[n];
    for(int i=0;i<n;i++)
    {
        arr[i] = new int[m];
    }
    return arr;
}

//release memory 1d
void release1d(int* arr)
{
    delete[] arr;
}

//release memory 2d
void release2d(int** arr, int n)
{
    for(int i=0;i<n;i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
}

bool validSequenceOfProcesses(int** needMat, int** alloc, int* availableVector)
{
    int nop = NUM_OF_Processes;
    int nor = NUM_OF_RESOURCE_TYPES;
    //to keep track of calculated processes
    bool* taken = new bool[nop];
    for(int i=0;i<nop;i++) taken[i] = 0;
    int currCnt = 0, oldCnt = -1;
    //order of the sequence we are getting
    vector<int> seq;
    while(currCnt != oldCnt)
    {
        oldCnt = currCnt;
        for(int i=0;i<nop;i++)
        {
            //if this process not taken then try to take it
            if(!taken[i])
            {
                bool canBeTaken = 1;
                for(int j=0;j<nor;j++)
                {
                    if(needMat[i][j] > availableVector[j])
                    {
                        canBeTaken = 0;
                        break;
                    }
                }
                if(canBeTaken)
                {
                    taken[i] = 1;
                    for(int j=0;j<nor;j++)
                    {
                        availableVector[j] += alloc[i][j];
                    }
                    seq.push_back(i);
                    currCnt++;
                }
            }
        }
    }
    delete[] taken;
/*
    //debugging the order of sequence we got
    cout<<"currCnt = "<<currCnt<<endl;
    cout<<"order \n";
    for(int i:seq){
        cout<<i<<' ';
    }
    cout<<endl;
*/
    return currCnt == nop;
}

bool isSafe(int pid, int* request)
{
    //check if request > Need
    for(int i=0;i<NUM_OF_RESOURCE_TYPES;i++)
    {
        if(request[i] > need[pid][i])
        {
            //process wants more than the claimed
            return false;
        }
    }

    //check if request > available
    for(int i=0;i<NUM_OF_RESOURCE_TYPES;i++)
    {
        if(request[i] > available[i])
        {
            //can not be granted not enough available instances
            return false;
        }
    }

    //available = available-request
    int* availableCpy = allocate1d(NUM_OF_RESOURCE_TYPES);
    cpy(available, availableCpy, NUM_OF_RESOURCE_TYPES);
    for(int i=0;i<NUM_OF_RESOURCE_TYPES;i++)
    {
        availableCpy[i] -= request[i];
    }

    //allocation of pid += request
    int** allocationCpy = allocate2d(NUM_OF_Processes, NUM_OF_RESOURCE_TYPES);
    cpy2d(allocation, allocationCpy, NUM_OF_Processes, NUM_OF_RESOURCE_TYPES);
    for(int i=0;i<NUM_OF_RESOURCE_TYPES;i++)
    {
        allocationCpy[pid][i] += request[i];
    }

    //need of pid -= request
    int** needCpy = allocate2d(NUM_OF_Processes, NUM_OF_RESOURCE_TYPES);
    cpy2d(need, needCpy, NUM_OF_Processes, NUM_OF_RESOURCE_TYPES);
    for(int i=0;i<NUM_OF_RESOURCE_TYPES;i++)
    {
        needCpy[pid][i] -= request[i];
    }

    //check for a valid sequence exists
    bool state = validSequenceOfProcesses(needCpy, allocationCpy, availableCpy);

    //release used memory
    release1d(availableCpy);
    release2d(allocationCpy, NUM_OF_Processes);
    release2d(needCpy, NUM_OF_Processes);

    return state;
}

int* getRequest()
{
    int* request = new int[NUM_OF_RESOURCE_TYPES];

    //fill the request
    for (int i=0; i<NUM_OF_RESOURCE_TYPES; i++)
    {
        cin>>request[i];
        //repeat the iteration if requesting a -ve number
        if(request[i]<0) i--;
    }
    return request;
}


void releaseResources(int pid)
{
    int *released = new int[NUM_OF_RESOURCE_TYPES];
    for (int i=0; i<NUM_OF_RESOURCE_TYPES; i++)
    {
        cin>>released[i];
        //repeat the iteration if releasing a -ve number
        if(released[i]<0) i--;
    }

    //add to available and remove from allocation
    for (int i=0; i<NUM_OF_RESOURCE_TYPES; i++)
    {
        available[i]+= min(allocation[pid][i],released[i]);
        allocation[pid][i] =max(0,allocation[pid][i] - released[i]);
    }
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

    string command;
    while (cin>>command)
    {
        if (command == "RQ")
        {
            int pid;
            cin>>pid;
            int *request = getRequest();
            bool is_safe = isSafe(pid, request);
        }

        else if(command == "RL")
        {
            int pid=0;
            cin>>pid;
            releaseResources(pid);
        }
        else
            return (0);
    }
    return 0;

}

//don't touch it :)
/*
TEST:
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
7 4 3
1 2 2
6 0 0
0 1 1
4 3 1
3 3 2

TEST:
0 0 1 2
1 0 0 0
1 3 5 4
0 6 3 2
0 0 1 4
0 0 0 0
0 7 5 0
1 0 0 2
0 0 2 0
0 6 4 2
1 5 2 0
*/
