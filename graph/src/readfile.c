#include "readfile.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool correctInputCities(unsigned int city, unsigned int size)
{
    return city <= size && city > 0;
}

Graph* readfile(unsigned int** capitals, unsigned int* len, char* inputfile)
{
    FILE* file = fopen(inputfile, "r");

    if (file == NULL) {
        return NULL;
    }

    unsigned int n, m;
    if (fscanf(file, "%u %u", &n, &m) != 2) {
        printf("incorrect input data format");
        fclose(file);
        return NULL;
    }

    unsigned int* cntVertex = calloc(n, sizeof(unsigned int));
    if (cntVertex == NULL) {
        fclose(file);
        return NULL;
    }

    for (unsigned int i = 0; i < m; i++) {
        unsigned city1, city2;
        int len;
        if (fscanf(file, "%u %u %d", &city1, &city2, &len) != 3
            || correctInputCities(city1, n) == 0
            || correctInputCities(city2, n) == 0) {
            printf("incorrect input data format");
            free(cntVertex);
            fclose(file);
            return NULL;
        }
        cntVertex[city1 - 1]++;
        cntVertex[city2 - 1]++;
    }

    rewind(file);
    fscanf(file, "%u %u", &n, &m);
    Graph* graph = createGraph(n);
    if (graph == NULL) {
        free(cntVertex);
        fclose(file);
        return NULL;
    }

    for (unsigned int i = 0; i < n; i++) {
        graph->vertex[i] = createVertex(cntVertex[i], i);
        if (graph->vertex[i] == NULL) {
            free(cntVertex);
            deleteGraph(graph);
            fclose(file);
            return NULL;
        }
    }

    for (unsigned int i = 0; i < m; i++) {
        unsigned city1, city2;
        int len;
        fscanf(file, "%u %u %d", &city1, &city2, &len);
        connectVertex(graph, city1, city2, len);
    }

    unsigned int cntStates;
    if (fscanf(file, "%u", &cntStates) != 1) {
        printf("incorrect input data format");
        free(cntVertex);
        deleteGraph(graph);
        fclose(file);
        return NULL;
    }

    unsigned int* caps = malloc(cntStates * sizeof(unsigned int));
    if (caps == NULL) {
        free(cntVertex);
        deleteGraph(graph);
        fclose(file);
        return NULL;
    }

    for (unsigned int i = 0; i < cntStates; i++) {
        unsigned int capital;
        if (fscanf(file, "%u", &capital) != 1 || !correctInputCities(capital, n)) {
            printf("incorrect input data format\n");
            free(caps);
            free(cntVertex);
            deleteGraph(graph);
            fclose(file);
            return NULL;
        }
        caps[i] = capital;
    }
    free(cntVertex);
    fclose(file);

    *capitals = caps;
    *len = cntStates;
    return graph;
}
