#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

static void siftUp(Heap* heap, unsigned int index)
{
    if (heap == NULL || heap->arrayNode == NULL) {
        return;
    }
    while (index > 0) {
        unsigned int parent = (index - 1) / 2;
        if (heap->arrayNode[parent].dist > heap->arrayNode[index].dist) {
            Node temp = heap->arrayNode[parent];
            heap->arrayNode[parent] = heap->arrayNode[index];
            heap->arrayNode[index] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

static void siftDown(Heap* heap, unsigned int index)
{
    if (heap == NULL || heap->arrayNode == NULL) {
        return;
    }
    while (index * 2 + 1 < heap->len) {
        unsigned int indexChild = index * 2 + 1;
        if (indexChild + 1 < heap->len && heap->arrayNode[indexChild].dist > heap->arrayNode[indexChild + 1].dist) {
            indexChild += 1;
        }
        if (heap->arrayNode[indexChild].dist < heap->arrayNode[index].dist) {
            Node temp = heap->arrayNode[indexChild];
            heap->arrayNode[indexChild] = heap->arrayNode[index];
            heap->arrayNode[index] = temp;
            index = indexChild;
        } else {
            break;
        }
    }
}

Heap* createHeap()
{
    Heap* heap = malloc(sizeof(Heap));
    if (heap == NULL) {
        return NULL;
    }
    heap->len = 0;
    heap->capacity = 1;
    heap->arrayNode = calloc(1, sizeof(Node));
    if (heap->arrayNode == NULL) {
        free(heap);
        return NULL;
    }
    return heap;
}

void insertHeap(Heap* heap, unsigned int city, int dist)
{
    if (heap == NULL || heap->arrayNode == NULL) {
        return;
    }
    Node newNode;
    newNode.city = city;
    newNode.dist = dist;
    if (heap->len == heap->capacity) {
        Node* newArrayNode = realloc(heap->arrayNode, sizeof(Node) * heap->capacity * 2);
        if (newArrayNode == NULL) {
            return;
        }
        heap->capacity *= 2;
        heap->arrayNode = newArrayNode;
    }
    heap->arrayNode[heap->len] = newNode;
    heap->len++;
    siftUp(heap, heap->len - 1);
}

void extractMinHeap(Heap* heap)
{
    if (heap == NULL || heap->arrayNode == NULL || heap->len == 0) {
        return;
    }
    heap->arrayNode[0] = heap->arrayNode[heap->len - 1];
    heap->len--;
    siftDown(heap, 0);
}

Node* getMin(Heap* heap)
{
    if (heap == NULL || heap->arrayNode == NULL || heap->len == 0) {
        return NULL;
    }
    return &(heap->arrayNode[0]);
}

void deleteHeap(Heap* heap)
{
    if (heap == NULL) {
        return;
    }
    if (heap->arrayNode == NULL) {
        free(heap);
        return;
    }
    free(heap->arrayNode);
    free(heap);
}
