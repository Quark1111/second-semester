#include "csv_printer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* string;
    int length;
    bool number;
} Cell;

struct Table {
    int rows;
    int columns;
    int* maxLengthColumns;
    char** data;
    Cell* cells;
};

static int countColumns(const char* buffer)
{
    int count = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ',' || buffer[i] == '\n') {
            count++;
        }
    }
    return count;
}

static bool isNumber(const char* string)
{
    if (string == NULL || *string == '\0')
        return false;

    char* endPointer;
    strtod(string, &endPointer);
    return (endPointer != string && *endPointer == '\0');
}

void deleteTable(Table* table)
{
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < table->rows; i++) {
        if (table->data[i] != NULL) {
            free(table->data[i]);
        }
    }
    free(table->data);

    if (table->cells != NULL) {
        for (int i = 0; i < table->columns * table->rows; i++) {
            if (table->cells[i].string != NULL) {
                free(table->cells[i].string);
            }
        }
        free(table->cells);
    }

    if (table->maxLengthColumns != NULL) {
        free(table->maxLengthColumns);
    }

    free(table);
}

static void drawLine(FILE* file, char symbol, const int* maxLengthColumns, int length)
{
    for (int i = 0; i < length; i++) {
        fprintf(file, "+");
        for (int j = 0; j < maxLengthColumns[i] + 2; j++) {
            fprintf(file, "%c", symbol);
        }
    }
    fprintf(file, "+\n");
}

void drawTable(const Table* table, const char* outputfile)
{
    if (table == NULL) {
        printf("Error: Table is NULL\n");
        return;
    }

    if (table->cells == NULL) {
        printf("Error: Table cells are not initialized\n");
        return;
    }

    if (table->maxLengthColumns == NULL) {
        printf("Error: Column widths are not initialized\n");
        return;
    }

    FILE* file = fopen(outputfile, "w");
    if (file == NULL) {
        printf("Error: Cannot create file %s\n", outputfile);
        return;
    }

    if (table->rows == 0) {
        drawLine(file, '=', table->maxLengthColumns, table->columns);
        fprintf(file, "|");
        for (int j = 0; j < table->columns; j++) {
            fprintf(file, " ");
            for (int k = 0; k < table->maxLengthColumns[j]; k++) {
                fprintf(file, " ");
            }
            fprintf(file, " |");
        }
        fprintf(file, "\n");
        drawLine(file, '=', table->maxLengthColumns, table->columns);
        fclose(file);
        return;
    }

    drawLine(file, '=', table->maxLengthColumns, table->columns);
    fprintf(file, "|");

    for (int j = 0; j < table->columns; j++) {
        int cellIndex = 0 * table->columns + j;
        Cell current = table->cells[cellIndex];
        fprintf(file, " ");
        if (current.string != NULL) {
            fprintf(file, "%s", current.string);
            for (int k = current.length; k < table->maxLengthColumns[j]; k++) {
                fprintf(file, " ");
            }
        } else {
            for (int k = 0; k < table->maxLengthColumns[j]; k++) {
                fprintf(file, " ");
            }
        }
        fprintf(file, " |");
    }

    fprintf(file, "\n");
    drawLine(file, '=', table->maxLengthColumns, table->columns);

    for (int i = 1; i < table->rows; i++) {
        fprintf(file, "|");
        for (int j = 0; j < table->columns; j++) {
            int cellIndex = i * table->columns + j;
            Cell current = table->cells[cellIndex];
            fprintf(file, " ");

            if (current.string != NULL) {
                if (current.number) {
                    for (int k = current.length; k < table->maxLengthColumns[j]; k++) {
                        fprintf(file, " ");
                    }
                    fprintf(file, "%s", current.string);
                } else {
                    fprintf(file, "%s", current.string);
                    for (int k = current.length; k < table->maxLengthColumns[j]; k++) {
                        fprintf(file, " ");
                    }
                }
            } else {
                for (int k = 0; k < table->maxLengthColumns[j]; k++) {
                    fprintf(file, " ");
                }
            }

            fprintf(file, " |");
        }

        fprintf(file, "\n");
        drawLine(file, '-', table->maxLengthColumns, table->columns);
    }

    fclose(file);
}

static Table* readCSV(const char* inputfile)
{
    FILE* file = fopen(inputfile, "r");
    if (file == NULL) {
        printf("File not found: %s\n", inputfile);
        return NULL;
    }

    Table* table = calloc(1, sizeof(Table));
    if (table == NULL) {
        fclose(file);
        return NULL;
    }

    int length = 0;
    int capacity = 10;
    int columns = 0;
    char** strings = malloc(capacity * sizeof(char*));
    if (strings == NULL) {
        free(table);
        fclose(file);
        return NULL;
    }

    char* buffer = NULL;
    size_t bufferSize = 0;

    while (getline(&buffer, &bufferSize, file) != -1) {
        if (length >= capacity) {
            capacity *= 2;
            char** newStrings = realloc(strings, capacity * sizeof(char*));
            if (newStrings == NULL) {
                printf("Error: Out of memory\n");
                free(buffer);
                for (int i = 0; i < length; i++) {
                    free(strings[i]);
                }
                free(strings);
                free(table);
                fclose(file);
                return NULL;
            }
            strings = newStrings;
        }

        strings[length] = buffer;
        length++;

        int newColumns = countColumns(buffer);
        if (newColumns > columns) {
            columns = newColumns;
        }

        buffer = NULL;
    }

    free(buffer);
    fclose(file);

    table->data = strings;
    table->columns = columns;
    table->rows = length;

    return table;
}

Table* createTable(const char* inputfile)
{
    Table* table = readCSV(inputfile);
    if (table == NULL) {
        return NULL;
    }

    if (table->columns > 0) {
        table->maxLengthColumns = calloc(table->columns, sizeof(int));
        if (table->maxLengthColumns == NULL) {
            printf("Error: Failed to allocate column widths\n");
            deleteTable(table);
            return NULL;
        }
    } else {
        table->maxLengthColumns = NULL;
    }

    if (table->rows > 0 && table->columns > 0) {
        table->cells = calloc(table->rows * table->columns, sizeof(Cell));
        if (table->cells == NULL) {
            printf("Error: Failed to allocate table cells\n");
            deleteTable(table);
            return NULL;
        }

        for (int i = 0; i < table->rows; i++) {
            int start = 0;
            int column = 0;
            char* row = table->data[i];
            size_t rowLength = strlen(row);

            for (size_t j = 0; j <= rowLength - 1; j++) {
                if (j == rowLength || row[j] == ',' || row[j] == '\n') {
                    int columnLength = (int)(j - start);
                    int cellIndex = i * table->columns + column;

                    table->cells[cellIndex].string = calloc(columnLength + 1, sizeof(char));
                    if (table->cells[cellIndex].string == NULL) {
                        printf("Error: Failed to allocate cell string\n");
                        deleteTable(table);
                        return NULL;
                    }

                    strncpy(table->cells[cellIndex].string, row + start, columnLength);
                    table->cells[cellIndex].string[columnLength] = '\0';
                    table->cells[cellIndex].length = columnLength;
                    table->cells[cellIndex].number = isNumber(table->cells[cellIndex].string);

                    if (table->maxLengthColumns[column] < columnLength) {
                        table->maxLengthColumns[column] = columnLength;
                    }

                    column++;
                    start = (int)j + 1;
                }
            }

            for (int j = column; j < table->columns; j++) {
                int cellIndex = i * table->columns + j;
                table->cells[cellIndex].string = NULL;
                table->cells[cellIndex].length = 0;
                table->cells[cellIndex].number = false;
            }
        }
    } else {
        table->cells = NULL;
    }

    return table;
}
