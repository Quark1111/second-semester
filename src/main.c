#include "csv_printer.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc == 3) {
        Table* table = createTable(argv[1]);
        if (table != NULL) {
            drawTable(table, argv[2]);
            deleteTable(table);
        }
        return 0;
    }

    if (argc == 1) {
        char input[256], output[256];
        printf("Enter input file: ");
        scanf("%s", input);
        printf("Enter output file: ");
        scanf("%s", output);
        Table* table = createTable(input);
        if (table != NULL) {
            drawTable(table, output);
            deleteTable(table);
        }
        return 0;
    }
    return 0;
}
