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

}


int main()
{
    greetingMessage();
    initDataStructures();
}
