#include "hash.h"
#include "heap.h"
#include "graph.h"
#include <chrono>
using namespace std;

int main()
{
    //step 1. Build the graph from the entered graph file
    
    // //Request input from user
    string graphFile;
    cout<<"Enter name of graph file: ";
    cin >> graphFile;
    //load the file into the graph

    graph g;

    loadGraph(graphFile, g);

    //step 2. Enter the starting vertex
    string vstart;
    cout<< "Enter name of starting vertex: ";
    cin >> vstart;
    
    //step 3. Run Dijkstras algorithm and them print time to complete the task
    auto start = chrono::high_resolution_clock::now();

    graph::dijkstras alg1(g, vstart);
   
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << duration.count() / 1000000.0 << std::endl;

    //step 4. output the results of dijsktras alg to a text file
    string outputFile;
    cout << "Enter name of output file: ";
    cin >> outputFile;

    alg1.exportResults(outputFile);
}
