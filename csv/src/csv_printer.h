#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef struct Table Table;

// Создаёт таблицу из CSV-файла.
Table* createTable(const char* inputfile);

// Удаляет таблицу и освобождает память.
void deleteTable(Table* table);

// Сохраняет таблицу в текстовый файл.
void drawTable(const Table* table, const char* outputfile);
