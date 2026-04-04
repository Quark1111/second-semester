#pragma once

typedef struct Vertex {
    unsigned int id;
    unsigned int* neighbors;
    int* weightNeighbors;
    unsigned int added;
    unsigned int cntNeighbors;
} Vertex;

typedef struct Graph {
    Vertex** vertex;
    unsigned int size;
} Graph;

Graph* createGraph(unsigned int size);
Vertex* createVertex(unsigned int cntNeighbors, unsigned int id);
void deleteGraph(Graph* graph);
void connectVertex(Graph* graph, unsigned int vertex1, unsigned int vertex2, int weight);