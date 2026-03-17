#include "csv_printer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int countColumns(char* buffer)
{
    int cnt = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ',' || buffer[i] == '\n') {
            cnt++;
        }
    }
    return cnt;
}

static bool isNumber(char* str)
{
    if (str == NULL || *str == '\0')
        return false;

    char* endptr;
    strtod(str, &endptr);
    return (endptr != str && *endptr == '\0');
}

void deleteTable(Table* table)
{
    if (table == NULL) {
        return;
    }

    for (int i = 0; i < table->row; i++) {
        if (table->data[i] != NULL) {
            free(table->data[i]);
        }
    }
    free(table->data);

    if (table->tabl != NULL) {
        for (int i = 0; i < table->cols * table->row; i++) {
            if (table->tabl[i].str != NULL) {
                free(table->tabl[i].str);
            }
        }
        free(table->tabl);
    }

    if (table->maxlenCols != NULL) {
        free(table->maxlenCols);
    }

    free(table);
}

static void drawLine(FILE* file, char symb, const int* maxCols, int len)
{
    for (int i = 0; i < len; i++) {
        fprintf(file, "+");
        for (int j = 0; j < maxCols[i] + 2; j++) {
            fprintf(file, "%c", symb);
        }
    }
    fprintf(file, "+\n");
}

void drawTable(Table* table, char* outputfile)
{
    if (table == NULL) {
        printf("Error: Table is NULL\n");
        return;
    }

    if (table->tabl == NULL) {
        printf("Error: Table cells are not initialized\n");
        return;
    }

    if (table->maxlenCols == NULL) {
        printf("Error: Column widths are not initialized\n");
        return;
    }

    FILE* file = fopen(outputfile, "w");
    if (file == NULL) {
        printf("Error: Cannot create file %s\n", outputfile);
        return;
    }

    if (table->row == 0) {
        drawLine(file, '=', table->maxlenCols, table->cols);
        fprintf(file, "|");
        for (int j = 0; j < table->cols; j++) {
            fprintf(file, " ");
            for (int k = 0; k < table->maxlenCols[j]; k++) {
                fprintf(file, " ");
            }
            fprintf(file, " |");
        }
        fprintf(file, "\n");
        drawLine(file, '=', table->maxlenCols, table->cols);
        fclose(file);
        return;
    }

    drawLine(file, '=', table->maxlenCols, table->cols);
    fprintf(file, "|");

    for (int j = 0; j < table->cols; j++) {
        int cellIndex = 0 * table->cols + j;
        Cage current = table->tabl[cellIndex];
        fprintf(file, " ");
        if (current.str != NULL) {
            fprintf(file, "%s", current.str);
            for (int k = current.len; k < table->maxlenCols[j]; k++) {
                fprintf(file, " ");
            }
        } else {
            for (int k = 0; k < table->maxlenCols[j]; k++) {
                fprintf(file, " ");
            }
        }
        fprintf(file, " |");
    }

    fprintf(file, "\n");
    drawLine(file, '=', table->maxlenCols, table->cols);

    for (int i = 1; i < table->row; i++) {
        fprintf(file, "|");
        for (int j = 0; j < table->cols; j++) {
            int cellIndex = i * table->cols + j;
            Cage current = table->tabl[cellIndex];
            fprintf(file, " ");

            if (current.str != NULL) {
                if (current.number) {
                    for (int k = current.len; k < table->maxlenCols[j]; k++) {
                        fprintf(file, " ");
                    }
                    fprintf(file, "%s", current.str);
                } else {
                    fprintf(file, "%s", current.str);
                    for (int k = current.len; k < table->maxlenCols[j]; k++) {
                        fprintf(file, " ");
                    }
                }
            } else {
                for (int k = 0; k < table->maxlenCols[j]; k++) {
                    fprintf(file, " ");
                }
            }

            fprintf(file, " |");
        }

        fprintf(file, "\n");
        drawLine(file, '-', table->maxlenCols, table->cols);
    }

    fclose(file);
    printf("Table successfully saved to %s\n", outputfile);
}

static Table* readCSV(char* inputfile)
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

    int len = 0, capacity = 10, cols = 0;
    char** str = malloc(capacity * sizeof(char*));
    if (str == NULL) {
        free(table);
        fclose(file);
        return NULL;
    }

    char* buffer = NULL;
    size_t ln = 0;

    while (getline(&buffer, &ln, file) != -1) {
        if (len >= capacity) {
            capacity *= 2;
            char** newstr = realloc(str, capacity * sizeof(char*));
            if (newstr == NULL) {
                printf("Error: Out of memory\n");
                free(buffer);
                for (int i = 0; i < len; i++) {
                    free(str[i]);
                }
                free(str);
                free(table);
                fclose(file);
                return NULL;
            }
            str = newstr;
        }

        str[len] = buffer;
        len++;

        int newcols = countColumns(buffer);
        if (newcols > cols) {
            cols = newcols;
        }

        buffer = NULL;
    }

    free(buffer);
    fclose(file);

    table->data = str;
    table->cols = cols;
    table->row = len;

    return table;
}

Table* createTable(char* inputfile)
{
    Table* table = readCSV(inputfile);
    if (table == NULL) {
        return NULL;
    }

    if (table->cols > 0) {
        table->maxlenCols = calloc(table->cols, sizeof(int));
        if (table->maxlenCols == NULL) {
            printf("Error: Failed to allocate column widths\n");
            deleteTable(table);
            return NULL;
        }
    } else {
        table->maxlenCols = NULL;
    }

    if (table->row > 0 && table->cols > 0) {
        table->tabl = calloc(table->row * table->cols, sizeof(Cage));
        if (table->tabl == NULL) {
            printf("Error: Failed to allocate table cells\n");
            deleteTable(table);
            return NULL;
        }

        for (int i = 0; i < table->row; i++) {
            int start = 0, col = 0;
            char* row = table->data[i];
            size_t rowLen = strlen(row);

            for (size_t j = 0; j <= rowLen - 1; j++) {
                if (j == rowLen || row[j] == ',' || row[j] == '\n') {
                    int lenCol = (int)(j - start);
                    int cellIndex = i * table->cols + col;

                    table->tabl[cellIndex].str = calloc(lenCol + 1, sizeof(char));
                    if (table->tabl[cellIndex].str == NULL) {
                        printf("Error: Failed to allocate cell string\n");
                        deleteTable(table);
                        return NULL;
                    }

                    strncpy(table->tabl[cellIndex].str, row + start, lenCol);
                    table->tabl[cellIndex].str[lenCol] = '\0';
                    table->tabl[cellIndex].len = lenCol;
                    table->tabl[cellIndex].number = isNumber(table->tabl[cellIndex].str);

                    if (table->maxlenCols[col] < lenCol) {
                        table->maxlenCols[col] = lenCol;
                    }

                    col++;
                    start = (int)j + 1;
                }
            }

            for (int j = col; j < table->cols; j++) {
                int cellIndex = i * table->cols + j;
                table->tabl[cellIndex].str = NULL;
                table->tabl[cellIndex].len = 0;
                table->tabl[cellIndex].number = false;
            }
        }
    } else {
        table->tabl = NULL;
    }

    return table;
}
