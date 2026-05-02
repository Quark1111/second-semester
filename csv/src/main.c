#include "csv_printer.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: %s <input.csv> <output.txt>\n", argv[0]);
        return 1;
    }
    Table* table = createTable(argv[1]);
    if (table == NULL) {
        return 1;
    }
    drawTable(table, argv[2]);
    deleteTable(table);
    return 0;
}
