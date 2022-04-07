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

//utilities
void cpy(int *src, int *dest, int sz) {
    for (int i = 0; i < sz; i++) {
        dest[i] = src[i];
    }
}

//cpy for 2d
void cpy2d(int **src, int **dest, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

//process_request memory 1d
int *allocate1d(int n) {
    return new int[n];
}

//process_request memory 2d
int **allocate2d(int n, int m) {
    int **arr = new int *[n];
    for (int i = 0; i < n; i++) {
        arr[i] = new int[m];
    }
    return arr;
}

//release memory 1d
void release1d(int *arr) {
    delete[] arr;
}

//release memory 2d
void release2d(int **arr, int n) {
    for (int i = 0; i < n; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}
//end of utilities

void greetingMessage() {
    cout << "*******************************\n";
    cout << "    Initialising System...\n";
    cout << "*******************************\n\n";
}


void initDataStructures() {
    cout << "*** Initializing resources ***\n";
    cout << "How many resources are in the system?\n";
    cin >> NUM_OF_RESOURCE_TYPES;

    available = allocate1d(NUM_OF_RESOURCE_TYPES); //Init available[]

    //Fill available[]
    cout << "Please enter the amount of initially available resources:\n";
    for (int i = 0; i < NUM_OF_RESOURCE_TYPES; i++) {
        printf("R%d:", i);
        cin >> available[i];
    }


    cout << "\n\n*** Initializing resources ***\n";
    cout << "How many processes are in the system?\n";
    cin >> NUM_OF_Processes;

    //init maximum[][]
    maximum = allocate2d(NUM_OF_Processes, NUM_OF_RESOURCE_TYPES);

    //init allocation[][]
    allocation = allocate2d(NUM_OF_Processes, NUM_OF_RESOURCE_TYPES);

    //init need[][]
    need = allocate2d(NUM_OF_Processes, NUM_OF_RESOURCE_TYPES);


    //fill maximum[][] and allocation[][] and need[][]
    cout << "For each process, enter resource info for each resource\n";
    cout << "space separated like: R0 R1 R2...\n";
    for (int i = 0; i < NUM_OF_Processes; i++) {
        //fill maximum[][]
        cout << "Process #" << i << " has maximum resources needs of: \n";
        for (int j = 0; j < NUM_OF_RESOURCE_TYPES; j++) {
            cin >> maximum[i][j];

            /*
            //check if unrecoverable deadlock exists
            if (maximum[i][j] > available[j]) {
                cout << "invalid\n";
            }
            */
        }

        //fill allocation[][]
        cout << "Process #" << i << " is already using resources of: \n";
        for (int j = 0; j < NUM_OF_RESOURCE_TYPES; j++)
            cin >> allocation[i][j];

        //fill need[][]
        for (int j = 0; j < NUM_OF_RESOURCE_TYPES; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];

        cout << "\n";
    }

    cout << "Initialisation completed.\n";
}

void printMatrix(int **mat) {
    for (int i = 0; i < NUM_OF_Processes; i++) {
        for (int j = 0; j < NUM_OF_RESOURCE_TYPES; j++)
            cout << mat[i][j] << " ";
        cout << "\n";
    }
    cout << '\n';
}


//copy sz blocks from src to dest

void allocate(int pid, int *request) {
    for (int i = 0; i < NUM_OF_RESOURCE_TYPES; i++) {
        allocation[pid][i] += request[i];
        available[i] -= request[i];
        need[pid][i] = maximum[pid][i] - allocation[pid][i];
    }
}

bool isSafe(int *availableVector) {
    int nop = NUM_OF_Processes;
    int nor = NUM_OF_RESOURCE_TYPES;
    //to keep track of calculated processes
    bool *finished = new bool[nop];
    for (int i = 0; i < nop; i++) finished[i] = 0;
    int currCnt = 0, oldCnt = -1;
    //order of the sequence we are getting
    vector<int> seq;
    while (currCnt != oldCnt) {
        oldCnt = currCnt;
        for (int i = 0; i < nop; i++) {
            //if this process not finished then try to take it
            if (!finished[i]) {
                bool canBeTaken = 1;
                for (int j = 0; j < nor; j++) {
                    if (need[i][j] > availableVector[j]) {
                        canBeTaken = 0;
                        break;
                    }
                }
                if (canBeTaken) {
                    finished[i] = 1;
                    for (int j = 0; j < nor; j++) {
                        availableVector[j] += allocation[i][j];
                    }
                    seq.push_back(i);
                    currCnt++;
                }
            }
        }
    }
    delete[] finished;
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

bool process_request(int pid, int *request) {
    //check if request > Need
    for (int i = 0; i < NUM_OF_RESOURCE_TYPES; i++) {
        if (request[i] > need[pid][i]) {
            //process wants more than the claimed
            return false;
        }
    }

    //check if request > available
    for (int i = 0; i < NUM_OF_RESOURCE_TYPES; i++) {
        if (request[i] > available[i]) {
            //can not be granted not enough available instances
            return false;
        }
    }

    allocate(pid, request);

    return true;
}

int *getRequest() {
    int *request = new int[NUM_OF_RESOURCE_TYPES];

    //fill the request
    for (int i = 0; i < NUM_OF_RESOURCE_TYPES; i++) {
        cin >> request[i];
        //repeat the iteration if requesting a -ve number
        if (request[i] < 0) i--;
    }
    return request;
}

void releaseResources(int pid) {
    int *released = new int[NUM_OF_RESOURCE_TYPES];
    for (int i = 0; i < NUM_OF_RESOURCE_TYPES; i++) {
        cin >> released[i];
        //repeat the iteration if releasing a -ve number
        if (released[i] < 0) i--;
    }

    //add to available and remove from allocation
    for (int i = 0; i < NUM_OF_RESOURCE_TYPES; i++) {
        available[i] += min(allocation[pid][i], released[i]);
        if (allocation[pid][i] < released[i])
        {
            cout<<"Warning:\nResource #"<<i<<" for pid= "<<pid<<" has less instances than the release request\n";
            cout<<allocation[pid][i]<<" < "<< released[i]<<endl;
        }
        allocation[pid][i] = max(0, allocation[pid][i] - released[i]);


    }
}

int selectVictim() {
    int victum = 0;
    int maxSum = 0;
    for (int p = 0; p < NUM_OF_Processes; p++) {
        int sum = 0;
        for (int r = 0; r < NUM_OF_RESOURCE_TYPES; r++) {
            sum += allocation[p][r];
        }
        if (sum > maxSum) {
            maxSum = sum;
            victum = p;
        }
    }

    for (int r = 0; r < NUM_OF_RESOURCE_TYPES; r++) {
        available[r] += allocation[victum][r];
        allocation[victum][r] = 0;
        need[victum][r] = maximum[victum][r];
    }
    return victum;
}

void recover() {
    while (true) {

        int *availableCpy = allocate1d(NUM_OF_RESOURCE_TYPES);
        cpy(available, availableCpy, NUM_OF_RESOURCE_TYPES);

        //release used memory

        if (isSafe(availableCpy)) {
            release1d(availableCpy);
            break;
        }

        release1d(availableCpy);

        int pEvicted = selectVictim();
        cout << "evicted process #" << pEvicted << "\n";
    }
}

void printAvailable() {
    for (int r = 0; r < NUM_OF_RESOURCE_TYPES; r++)
        cout << available[r] << " ";
    cout << "\n";
}

void deinitDataStructures() {
    release1d(available);
    release2d(maximum, NUM_OF_Processes);
    release2d(need, NUM_OF_Processes);
    release2d(allocation, NUM_OF_Processes);
}

int main() {

    greetingMessage();
    initDataStructures();

    string command;
    printAvailable();
    while (cin >> command) {
        if (command == "RQ") {
            int pid;
            cin >> pid;
            int *request = getRequest();
            bool allocated = process_request(pid, request);
            if (allocated) {
                cout << "allocated successfully\n";
                recover();
                cout<<"Allocation:\n";
                printMatrix(allocation);
                cout<<"Available:\n";
                printAvailable();
            } else {
                cout << "not enough resources\n";
            }

        } else if (command == "RL") {
            int pid = 0;
            cin >> pid;
            releaseResources(pid);
            cout<<"Allocation:\n";
            printMatrix(allocation);
            cout<<"Available:\n";
            printAvailable();

        } else
            break;
    }
    deinitDataStructures();
    return 0;

}



/*
    3
    1 1 1
    2
    0 1 1
    0 0 1
    1 0 0
    1 0 0
    RQ 0 0 1 0
    RL 0 0 1 1
    RL 1 1 0 5

 */

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
