#include "csv_printer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static int cntcols(char* buffer)
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

void DeleteTable(Table* table)
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
    
    for (int i = 0; i < table->cols * table->row; i++) {
        if (table->Tabl[i].str != NULL) {
            free(table->Tabl[i].str);
        }
    }
    free(table->Tabl);
    
    if (table->maxlenCols != NULL) {
        free(table->maxlenCols);
    }
    
    free(table);
}

static void drawLine(FILE* file, char symb, int* maxCols, int len)
{
    for (int i = 0; i < len; i++) {
        fprintf(file, "+");
        for (int j = 0; j < maxCols[i] + 2; j++) {
            fprintf(file, "%c", symb);
        }
    }
    fprintf(file, "+\n");
}

void DrawTable(Table* table, char* outputfile)
{
    if (table == NULL) {
        printf("Error: Table is NULL\n");
        return;
    }
    
    if (table->Tabl == NULL) {
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
        int cell_index = 0 * table->cols + j;
        cage current = table->Tabl[cell_index];
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
            int cell_index = i * table->cols + j;
            cage current = table->Tabl[cell_index];
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
        
        int newcols = cntcols(buffer);
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

Table* Createtable(char* inputfile)
{
    Table *table = readCSV(inputfile);
    if (table == NULL) {
        return NULL;
    }
    
    table->maxlenCols = calloc(table->cols, sizeof(int));
    if (table->maxlenCols == NULL) {
        printf("Error: Failed to allocate column widths\n");
        DeleteTable(table);
        return NULL;
    }
    
    table->Tabl = calloc(table->row * table->cols, sizeof(cage));
    if (table->Tabl == NULL) {
        printf("Error: Failed to allocate table cells\n");
        DeleteTable(table);
        return NULL;
    }
    
    for (int i = 0; i < table->row; i++) {
        int start = 0, col = 0;
        char* row = table->data[i];
        int row_len = strlen(row);
        
        for (int j = 0; j <= row_len - 1; j++) {
            if (j == row_len || row[j] == ',' || row[j] == '\n') {
                int lenCol = j - start;
                int cell_index = i * table->cols + col;
                
                table->Tabl[cell_index].str = calloc(lenCol + 1, sizeof(char));
                if (table->Tabl[cell_index].str == NULL) {
                    printf("Error: Failed to allocate cell string\n");
                    DeleteTable(table);
                    return NULL;
                }
                
                strncpy(table->Tabl[cell_index].str, row + start, lenCol);
                table->Tabl[cell_index].str[lenCol] = '\0';
                table->Tabl[cell_index].len = lenCol;
                table->Tabl[cell_index].number = isNumber(table->Tabl[cell_index].str);
                
                if (table->maxlenCols[col] < lenCol) {
                    table->maxlenCols[col] = lenCol;
                }
                
                col++;
                start = j + 1;
            }
        }
        
        for (int j = col; j < table->cols; j++) {
            int cell_index = i * table->cols + j;
            table->Tabl[cell_index].str = NULL;
            table->Tabl[cell_index].len = 0;
            table->Tabl[cell_index].number = false;
        }
    }
    
    return table;
}


