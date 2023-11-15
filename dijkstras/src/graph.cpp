#include "../include/graph.h"

//constructor for a vertex in the graph
graph::vertex::vertex(const std::string &id){
    this ->id = id;
};

graph::vertex::edge::edge(graph::vertex* &destNode, int weight){
    this -> destNode = destNode;
    this -> weight = weight;
};

void graph::addEdge(const std::string &src, const std::string &dest, int weight){
    //if either the source or the destination is not in the graph already, add it in
    if(graphmap.contains(src)==false){ //source is not currently in the graph
        vertices.push_back(vertex(src));
        graphmap.insert(src, &vertices.back());
    }

    if(graphmap.contains(dest)==false){ //destination is not currently in the graph
        vertices.push_back(vertex(dest));
        graphmap.insert(dest, &vertices.back());
    }

    //get the pointers to the vertex objects in the graph
    vertex* sourceNode = (vertex*)graphmap.getPointer(src);
    vertex* destNode = (vertex*)graphmap.getPointer(dest);

    //add an edge to the adjecency list 
    sourceNode->adjList.push_back(vertex::edge(destNode, weight));
    
};

void graph::printGraph(){
    std::cout<< "Verticies in Graph:"<<std::endl;
    for(auto i: vertices){
        std::cout<< i.id<< ' ';
    }
    std::cout<<std::endl<<"Edges in Graph:" << std::endl;
    for(auto i: vertices){
        for(auto j: i.adjList){
            std::cout<< i.id << ' '<< j.destNode->id<< ' ' << j.weight << std::endl;
        }
    }
}

void loadGraph(const std::string graphFile, graph &graph){
    std::ifstream infile;
    infile.open(graphFile);

    if(infile.is_open())
    {
        std::string line;
        //for every line in the file, add the edge associated with the line into the graph
        while (getline(infile,line)){
            std::string wordList[3];
            int wordCount=0;
            
            //extract the 3 words from the line corresponding with the source, dest and weight
            //wordCount will only go to 3 if it is a valid string (which we can assume is true)
            for(int i =0; i<(int)line.length(); i++){
                if(line[i]!= ' '){
                    wordList[wordCount] += line[i];
                }
                else{
                    wordCount++;
                }
            }
            //add the edge asscociated with this line to the graph
            graph.addEdge(wordList[0], wordList[1], stoi(wordList[2]));
        }   
        infile.close();
    }
    else{
        std::cout<< "Unable to open graph input file"<< std::endl;
        return;
    }
}




