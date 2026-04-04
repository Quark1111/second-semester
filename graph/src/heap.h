#pragma once

typedef struct Node {
    unsigned int city;
    int dist;
} Node;

typedef struct Heap {
    unsigned int len;
    unsigned int capacity;
    Node* arrayNode;
} Heap;

Heap* createHeap();
void deleteHeap(Heap* heap);
void insertHeap(Heap* heap, unsigned int city, int dist);
Node* getMin(Heap* heap);
void extractMinHeap(Heap* heap);