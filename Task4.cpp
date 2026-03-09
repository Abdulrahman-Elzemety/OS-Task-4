#include <iostream>
#include <fstream>
using namespace std;


int main() {
    ifstream file("nodeadlock.txt");
    
    // Read number of processes and resource types
    int p, r;
    file >> p >> r;

    int E[10];       // existing resources in system
    int C[10][10];   // recources currently allocated each process
    int R[10][10];   // recource each process requesting
    int A[10];       // available recources in system
    bool finish[10]; // which process are finished

    // Existing resources 
    for(int i = 0; i < r; i++)
        file >> E[i];

    // Allocation matrix for each process
    for(int i = 0; i < p; i++)
        for(int j = 0; j < r; j++)
            file >> C[i][j];

    // Request matrix for each process
    for(int i = 0; i < p; i++)
        for(int j = 0; j < r; j++)
            file >> R[i][j];

    // Available resources (subtract allocations from existing resources)
    for(int j = 0; j < r; j++) {
        int used = 0;
        for(int i = 0; i < p; i++)
            used += C[i][j];
        A[j] = E[j] - used;
    }

    // Make all processes  unfinished
    for(int i = 0; i < p; i++)
        finish[i] = false;

    // Deadlock detection loops, keeps checking unfinished processes, see if their requests
    // can be satisfied with available resources and them finished
    for(int k = 0; k < p; k++) {
        for(int i = 0; i < p; i++) {
            if(finish[i] == false) {
                bool ok = true;

                for(int j = 0; j < r; j++) {
                    if(R[i][j] > A[j])
                        ok = false;
                }
                if(ok) {
                    for(int j = 0; j < r; j++)
                        A[j] += C[i][j];

                    finish[i] = true;
                }
            }
        }
    }

    // Check if ther is deadlock
    bool deadlock = false;

    for(int i = 0; i < p; i++)
        if(finish[i] == false)
            deadlock = true;

    // Print results
    if(deadlock) {
        cout << "Deadlock detected\nProcesses: ";
        for(int i = 0; i < p; i++)
            if(finish[i] == false)
                cout << "P" << i << " ";
    }
    else
        cout << "No deadlock";

    return 0;
}