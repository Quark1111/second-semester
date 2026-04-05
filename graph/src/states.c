#include "graph.h"
#include "heap.h"
#include "readfile.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void deleteAllHeap(Heap** heaps, unsigned int size)
{
    if (heaps == NULL) {
        return;
    }
    for (unsigned int i = 0; i < size; i++) {
        if (heaps[i] != NULL) {
            deleteHeap(heaps[i]);
        }
    }
    free(heaps);
}

int solve(Graph* graph, unsigned int* capitals, unsigned int len, char* outputfile)
{
    unsigned int n = graph->size;
    unsigned int* visited = malloc(n * sizeof(unsigned int));
    unsigned int cntVisited = 0;
    if (visited == NULL) {
        return -1;
    }

    for (unsigned int i = 0; i < n; i++) {
        visited[i] = 0;
    }

    Heap** allHeap = calloc(len, sizeof(Heap*));
    if (allHeap == NULL) {
        free(visited);
        return -1;
    }
    for (unsigned int i = 0; i < len; i++) {
        allHeap[i] = createHeap();
        if (allHeap[i] == NULL) {
            deleteAllHeap(allHeap, i);
            free(visited);
            return -1;
        }
        insertHeap(allHeap[i], capitals[i], 0);
    }

    unsigned int cntSkip = 0;
    while (cntVisited < n && cntSkip != len) {
        cntSkip = 0;
        for (unsigned int i = 0; i < len; i++) {
            int found = 0;
            while (allHeap[i]->len > 0 && !found) {
                Node node = *getMin(allHeap[i]);
                extractMinHeap(allHeap[i]);
                unsigned int city = node.city;
                unsigned int cityIdx = city - 1;

                if (visited[cityIdx] == 0) {
                    found = 1;
                    visited[cityIdx] = i + 1;
                    cntVisited++;

                    for (unsigned int j = 0; j < graph->vertex[cityIdx]->cntNeighbors; j++) {
                        unsigned int vertex = graph->vertex[cityIdx]->neighbors[j];
                        int weight = graph->vertex[cityIdx]->weightNeighbors[j];

                        if (!visited[vertex - 1]) {
                            insertHeap(allHeap[i], vertex, weight);
                        }
                    }
                }
            }

            if (!found) {
                cntSkip++;
            }
        }
    }

    for (unsigned int i = 0; i < len; i++) {
        deleteHeap(allHeap[i]);
    }

    FILE* file = fopen(outputfile, "w");
    if (file == NULL) {
        free(allHeap);
        free(visited);
        return -1;
    }
    for (unsigned int s = 0; s < len; s++) {
        fprintf(file, "State number %u has cities: ", s + 1);
        for (unsigned int i = 0; i < n; i++) {
            if (visited[i] == s + 1) {
                fprintf(file, "%u ", i + 1);
            }
        }
        fprintf(file, "\n");
    }

    free(allHeap);
    free(visited);
    fclose(file);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        return -1;
    }
    unsigned int* capitals = NULL;
    unsigned int len = 0;

    Graph* graph = readfile(&capitals, &len, argv[1]);

    if (graph == NULL) {
        printf("Error reading file\n");
        return 1;
    }
    if (capitals == NULL) {
        return -1;
    }
    int result = solve(graph, capitals, len, argv[2]);
    if (result == -1) {
        printf("error");
    }
    deleteGraph(graph);
    free(capitals);

    return result;
}
