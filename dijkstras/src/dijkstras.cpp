#include "graph.h"
#include "hash.h"
#include "heap.h"

graph::dijkstras::destVert::destVert(graph::vertex *dVert){
    this ->self = dVert;
}

graph::dijkstras::destVert *graph::dijkstras::getDestPtr(std::string id){
    return (destVert*)destMap.getPointer(id);
}


graph::dijkstras::dijkstras(graph &graph, std::string vStartName): destMap(2* graph.vertices.size()){
    //initialise heap of unknown vertices ordered by their distances to the starting vertex 
    heap unknownVs(graph.vertices.size()); //heap of known vertices ordered by their distances to the source
    
    //initialise all the vertices in the graph, default values for d and prev are (INT_MAX), and nullptr.
    for(auto &u: graph.vertices){
        destinations.push_back(destVert(&u));
        destVert* tempDest = &destinations.back(); 

        destMap.insert(u.id, tempDest);
        unknownVs.insert(u.id, INT_MAX, tempDest);
    }
    //Initialise the starting vertex with a distance of 0 in the list and also in the heap
    //it is also the first vertex searched so it can be set as vCurr
    destVert* vCurr = getDestPtr(vStartName);
    vCurr -> dist = 0;
    unknownVs.setKey(vStartName, 0);
    
    //storage variables for for loop below
    std::string vCurrName;
    int vCurrdist; 
    

    //while heap is not empty, update the distances to nodes adjecent to the node with the shortest distance
    //while there are still unkown vertices
    while(unknownVs.deleteMin(&vCurrName, &vCurrdist, &vCurr) != 1){
        //set known to true for the current vertex
        vCurr->known = true;

        //for each edge in the adjecency list of the current vertex
        for(auto edg: vCurr->self->adjList){
            
            //set a storage variable for the destination vertex of the edge
            destVert* edgeDest = getDestPtr(edg.destNode->id);

            if(edgeDest->dist > vCurr->dist + edg.weight && !edgeDest->known && vCurr->dist != INT_MAX){
                //update the distance of vCurr
                edgeDest->dist = vCurr->dist + edg.weight;
                unknownVs.setKey(edg.destNode->id, vCurr->dist);
                
                //update the previous node in the shortest path, and the explored status
                edgeDest->prevVert = vCurr;
            }
        }
    }
}  

void graph::dijkstras::exportResults(std::string outFile){
    std::ofstream output(outFile);
    for(auto v : this->destinations){
        output<< v.self->id <<": ";

        if(v.dist== INT_MAX){
            output<< "NO PATH"<<std::endl;
        }
        else{

            output<< v.dist<< " [";
            std::list<std::string> path = getPath(v.self->id);
           
            for(auto pos : path){
                if(pos != path.back()){
                    output<< pos<< ", ";
                }
                else{
                    output<<pos;
                }
            }
            output<< ']'<< std::endl;
        }
    }
    output.close();
}

std::list<std::string> graph::dijkstras::getPath(const std::string &destid){
    std::list<std::string> pathList;
    destVert* v = getDestPtr(destid);
    
    while(v->prevVert != nullptr){
        pathList.push_front(v->self->id);
        v = v->prevVert;
    }
    pathList.push_front(v->self->id);

    return pathList;
}
