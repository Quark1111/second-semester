#include "csv_printer.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (argc == 3) {
        Table* table = Createtable(argv[1]);
        if (table) {
            DrawTable(table, argv[2]);
            DeleteTable(table);
        }
    } else if (argc == 1) {
        char inputfile[256], outputfile[256];
        printf("Enter input file: ");
        if (scanf("%255s", inputfile) != 1) {
            return 1;
        }
        printf("Enter output file: ");
        if (scanf("%255s", outputfile) != 1) {
            return 1;
        }
        Table* table = Createtable(inputfile);
        if (table) {
            DrawTable(table, outputfile);
            DeleteTable(table);
        }
    } else {
        printf("Usage: %s [input.csv output.txt]\n", argv[0]);
        printf("   or: %s (interactive mode)\n", argv[0]);
        return 1;
    }
    return 0;
}
