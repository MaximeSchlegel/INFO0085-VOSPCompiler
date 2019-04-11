#include "graph.h"


Graph::Graph(int V) {
    this->V = V;
    this->adj = new std::list<int>[V];
}

void Graph::addEdge(int v, int w) {
    (this->adj)[v].push_back(w); // Add w to v’s list.
}

std::vector<int> *Graph::isCyclicUtil(int v, bool visited[], bool *recStack) {
    if(!visited[v]) {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;

        // Recur for all the vertices adjacent to this vertex
        std::list<int>::iterator i;
        std::vector<int> *path;
        for(i = this->adj[v].begin(); i != this->adj[v].end(); ++i) {
            if (!visited[*i] && (path = isCyclicUtil(*i, visited, recStack)) != nullptr) {
                if(path->size() == 1 || path[0] != path[path->size() - 1]) {
                    path->push_back(*i);
                }
                return path;
            } else if (recStack[*i]) {
                std::vector <int> *path = new std::vector<int>();
                path->push_back(*i);
                return path;
            }
        }
    }
    recStack[v] = false; // remove the vertex from recursion stack
    return nullptr;
}

// Returns the vector of the first cycle found if the graph contains a cycle, else nullptr.
std::vector<int> *Graph::isCyclic() {
    // Mark all the vertices as not visited and not part of recursion stack
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 0; i < V; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    // Call the recursive helper function to detect cycle in different DFS trees
    std::vector<int> *path;
    for(int i = 0; i < V; i++) {
        if (path = isCyclicUtil(i, visited, recStack)) {
            return path;
        }
    }
    return nullptr;
}