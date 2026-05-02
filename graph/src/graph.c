#include "graph.h"
#include <stdlib.h>

Vertex* createVertex(unsigned int cntNeighbors, unsigned int id)
{
    Vertex* vertex = calloc(1, sizeof(Vertex));
    if (vertex == NULL) {
        return NULL;
    }
    if (cntNeighbors != 0) {
        vertex->neighbors = calloc(cntNeighbors, sizeof(unsigned int));
        if (vertex->neighbors == NULL) {
            free(vertex);
            return NULL;
        }
        vertex->weightNeighbors = calloc(cntNeighbors, sizeof(int));
        if (vertex->weightNeighbors == NULL) {
            free(vertex->neighbors);
            free(vertex);
            return NULL;
        }
    } else {
        vertex->neighbors = NULL;
        vertex->weightNeighbors = NULL;
    }
    vertex->cntNeighbors = cntNeighbors;
    vertex->added = 0;
    vertex->id = id;
    return vertex;
}

void connectVertex(Graph* graph, unsigned int vertex1, unsigned int vertex2, int weight)
{
    if (graph == NULL || graph->vertex == NULL) {
        return;
    }
    if (vertex1 < 1 || vertex1 > graph->size || vertex2 < 1 || vertex2 > graph->size) {
        return;
    }
    if (graph->vertex[vertex1 - 1] == NULL || graph->vertex[vertex2 - 1] == NULL) {
        return;
    }

    unsigned int* addedVertex1 = &(graph->vertex[vertex1 - 1]->added);
    unsigned int* addedVertex2 = &(graph->vertex[vertex2 - 1]->added);

    graph->vertex[vertex1 - 1]->neighbors[*addedVertex1] = vertex2;
    graph->vertex[vertex1 - 1]->weightNeighbors[*addedVertex1] = weight;
    (*addedVertex1)++;

    graph->vertex[vertex2 - 1]->neighbors[*addedVertex2] = vertex1;
    graph->vertex[vertex2 - 1]->weightNeighbors[*addedVertex2] = weight;
    (*addedVertex2)++;
}

void deleteGraph(Graph* graph)
{
    if (graph == NULL) {
        return;
    }
    if (graph->vertex == NULL) {
        free(graph);
        return;
    }
    for (unsigned int i = 0; i < graph->size; i++) {
        if (graph->vertex[i] != NULL) {
            free(graph->vertex[i]->neighbors);
            free(graph->vertex[i]->weightNeighbors);
            free(graph->vertex[i]);
        }
    }
    free(graph->vertex);
    free(graph);
}

Graph* createGraph(unsigned int size)
{
    Graph* graph = calloc(1, sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }
    graph->size = size;
    graph->vertex = calloc(graph->size, sizeof(Vertex*));
    if (graph->vertex == NULL) {
        free(graph);
        return NULL;
    }
    return graph;
}
