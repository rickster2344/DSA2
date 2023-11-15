#pragma once
#include "heap.h"
#include "hash.h"
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <climits>


class graph{

    private:
        struct vertex{ //using a struct because there is no point in having any private data members in this structure
            
            //each vertex in the graph has an id, and an adjacency list of all outgoing edges
            struct edge{
                int weight; 
                vertex* destNode;

                //constructor for an edge 
                edge(vertex* &destNode, int weight);
            };

            std::string id;
            std::list<edge> adjList;

            vertex(const std::string &id);
       };

        std::list<vertex> vertices; //list of all the vertecies in the graph
        hashTable graphmap; //mapping of the vertecies in the graph to a hashtable to make referencing faster
    public:
        //adding the edge will automatically declare a new vertex when the vertex is not already in the graph
        void addEdge(const std::string &src, const std::string &dest, int weight);

    
        void printGraph(); //prints out all the current vertecies in the graph

        
        //default constructor for graph is used since no special methods needed for graph object. hashtable initialises to its smallest size (53) since 
        //int capacity is initialised to 0 when not specified.
        
        struct dijkstras{
            //DIJSKRAS MEMBER VARIABLES:
            struct destVert{
                int dist{INT_MAX};// distance to source node (always initialised to maximum integer value)
                bool known{false}; //always initialised to unknown.
                vertex* self{nullptr}; //pointer to itself 
                destVert* prevVert{nullptr}; //pointer to the previous vertex in the algorithm. 
                
                destVert(vertex* dVert);//constructor for the destination vert object, the pointer self is set to the passed variable dVert
            };

            hashTable destMap; //mapping of all the destintion vertices to their associated nodes
            std::list<destVert> destinations; //list of all the destination vertices and their distances, previous nodes, and explored status

            //DIJKSTRAS MEMBER FUNCTIONS:
            destVert* getDestPtr(std::string id);
            void exportResults(std::string outFile);
            std::list<std::string> getPath(const std::string &destid);
            
            dijkstras(graph &graph, std::string vStartName); //constructor for dijskra algorithm, which passes the starting vertex and the graph to be searched
        };
};

void loadGraph(const std::string graphFile, graph &graph);
