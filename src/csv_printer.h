#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct Cage {
    char* str;
    int len;
    bool number;
} Cage;

typedef struct {
    int row;
    int cols;
    int* maxlenCols;
    char** data;
    Cage* tabl;
} Table;

Table* createTable(char* inputfile);
void deleteTable(Table* table);
void drawTable(Table* table, char* outputfile);
