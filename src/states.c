#include "graph.h"
#include "heap.h"
#include "readfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

int minDistance(int weight1, int weight2)
{
    if (weight1 > weight2) {
        return weight2;
    }
    return weight1;
}

int solve(Graph* graph, unsigned int* capitals, unsigned int len, char* outputfile)
{
    unsigned int n = graph->size;
    unsigned int* visited = malloc(n * sizeof(unsigned int));
    unsigned int cntVisited = 0;
    if (visited == NULL) {
        return -1;
    }
    int* distance = malloc(n * sizeof(int));
    if (distance == NULL) {
        free(visited);
        return -1;
    }
    for (unsigned int i = 0; i < n; i++) {
        visited[i] = 0;
        distance[i] = INT_MAX;
    }
    
    printf("\n========== ИНИЦИАЛИЗАЦИЯ ==========\n");
    for (unsigned int i = 0; i < len; i++) {
        unsigned int capital = capitals[i];
        distance[capital - 1] = 0;
        printf("Гос %d: столица %d\n", i + 1, capital);
    }
    
    Heap** allHeap = calloc(len, sizeof(Heap*));
    if (allHeap == NULL) {
        free(visited);
        free(distance);
        return -1;
    }
    
    printf("\n========== СОЗДАНИЕ КУЧ ==========\n");
    for (unsigned int i = 0; i < len; i++) {
        allHeap[i] = createHeap();
        if (allHeap[i] == NULL) {
            deleteAllHeap(allHeap, i);
            free(visited);
            free(distance);
            return -1;
        }
        insertHeap(allHeap[i], capitals[i], 0);
        printf("Гос %d: добавлена столица %d\n", i + 1, capitals[i]);
    }
    
    printf("\n========== НАЧАЛО ОСНОВНОГО ЦИКЛА ==========\n");
    printf("Всего городов: %d\n", n);
    
    unsigned int cntSkip = 0;
    int step = 1;
    
    while (cntVisited < n && cntSkip != len) {
        printf("\n--- ШАГ %d ---\n", step++);
        printf("cntVisited=%d, cntSkip=%d, len=%d\n", cntVisited, cntSkip, len);
        cntSkip = 0;
        
        for (unsigned int i = 0; i < len; i++) {
            printf("\n  [Гос %d] Размер кучи: %d\n", i + 1, allHeap[i]->len);
            int found = 0;
            
            while (allHeap[i]->len > 0 && !found) {
                Node* node = getMin(allHeap[i]);
                if (node == NULL) {
                    extractMinHeap(allHeap[i]);
                    continue;
                }
                
                unsigned int city = node->city;
                unsigned int cityIdx = city - 1;
                
                printf("    Извлёк город %d (расст=%d), visited[%d]=%d\n", 
                       city, node->dist, cityIdx, visited[cityIdx]);
                
                if (visited[cityIdx] == 0) {
                    printf("    >>> ЗАХВАТЫВАЕТ город %d государством %d\n", city, i + 1);
                    found = 1;
                    visited[cityIdx] = i + 1;
                    cntVisited++;
                    
                    printf("        Добавляю соседей города %d:\n", city);
                    for (unsigned int j = 0; j < graph->vertex[cityIdx]->cntNeighbors; j++) {
                        unsigned int vertex = graph->vertex[cityIdx]->neighbors[j];
                        if (!visited[vertex - 1]) {
                            int newDist = distance[cityIdx] + graph->vertex[cityIdx]->weightNeighbors[j];
                            if (newDist < distance[vertex - 1]) {
                                distance[vertex - 1] = newDist;
                                insertHeap(allHeap[i], vertex, newDist);
                                printf("          -> добавил %d (расст=%d)\n", vertex, newDist);
                            }
                        }
                    }
                } else {
                    printf("    Пропускаю (уже захвачен)\n");
                }
                extractMinHeap(allHeap[i]);
            }
            
            if (!found) {
                cntSkip++;
                printf("  Гос %d: не нашёл город, cntSkip=%d\n", i + 1, cntSkip);
            }
        }
        
        printf("\n--- Состояние visited после шага %d ---\n", step - 1);
        for (unsigned int k = 0; k < n; k++) {
            if (visited[k] != 0) {
                printf("  город %d -> гос %d\n", k + 1, visited[k]);
            }
        }
    }
    
    printf("\n========== ЗАВЕРШЕНИЕ ==========\n");
    printf("Всего захвачено: %d из %d\n", cntVisited, n);
    
    for (unsigned int i = 0; i < len; i++) {
        deleteHeap(allHeap[i]);
    }
    
    FILE* file = fopen(outputfile, "w");
    if (file == NULL) {
        free(allHeap);
        free(visited);
        free(distance);
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
    free(distance);
    fclose(file);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    
    unsigned int* capitals = NULL;
    unsigned int len = 0;
    
    Graph* graph = readfile(&capitals, &len, argv[1]);
    
    if (graph == NULL) {
        printf("Error reading file\n");
        return 1;
    }
    if (capitals == NULL) {
        printf("Error: capitals is NULL\n");
        return 1;
    }
    
    printf("Граф загружен. Городов: %d, государств: %d\n", graph->size, len);
    printf("Столицы: ");
    for (unsigned int i = 0; i < len; i++) {
        printf("%d ", capitals[i]);
    }
    printf("\n");
    
    int result = solve(graph, capitals, len, argv[2]);
    if (result == -1) {
        printf("error\n");
    }
    
    deleteGraph(graph);
    free(capitals);
    
    return result;
}