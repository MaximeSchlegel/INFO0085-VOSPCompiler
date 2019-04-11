#ifndef VSOPCOMPILER_GRAPH_H
#define VSOPCOMPILER_GRAPH_H

#include <vector>
#include <list>


class Graph {
private:
    int V; // No. of vertices
    std::list<int> *adj; // Pointer to an array containing adjacency lists
    std::vector<int> *isCyclicUtil(int v, bool visited[], bool *rs); // used by isCyclic()

public:
    Graph(int V); // Constructor
    void addEdge(int v, int w); // to add an edge to graph
    std::vector<int> *isCyclic(); // returns true if there is a cycle in this graph
};

#endif //VSOPCOMPILER_GRAPH_H
