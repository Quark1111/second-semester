#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct cage {
    char* str;
    int len;
    bool number;
} cage;

typedef struct {
    int row;
    int cols;
    int* maxlenCols;
    char** data;
    cage* Tabl;
} Table;

Table* Createtable(char* inputfile);
void DeleteTable(Table* table);
void DrawTable(Table* table, char* outputfile);
