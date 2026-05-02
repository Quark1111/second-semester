#include "csv_printer.h"
#include <stdio.h>
#include <stdlib.h>

static int compareFiles(const char* file1, const char* file2)
{
    FILE* first = fopen(file1, "r");
    if (first == NULL) {
        return 0;
    }

    FILE* second = fopen(file2, "r");
    if (second == NULL) {
        fclose(first);
        return 0;
    }

    int character1 = 0;
    int character2 = 0;
    while (character1 != EOF && character2 != EOF) {
        character1 = fgetc(first);
        character2 = fgetc(second);
        if (character1 != character2) {
            fclose(first);
            fclose(second);
            return 0;
        }
    }

    fclose(first);
    fclose(second);
    return 1;
}

static void runTest(const char* inputFile, const char* expectedFile, int testNumber)
{
    char outputFile[256];
    sprintf(outputFile, "test_output_%d.txt", testNumber);

    Table* table = createTable(inputFile);
    if (table == NULL) {
        printf("Test %d: FAIL (cannot create table)\n", testNumber);
        return;
    }

    drawTable(table, outputFile);
    deleteTable(table);

    if (compareFiles(outputFile, expectedFile)) {
        printf("Test %d: OK\n", testNumber);
    } else {
        printf("Test %d: FAIL\n", testNumber);
    }

    remove(outputFile);
}

int main()
{
    printf("Running CSV printer tests:\n");

    runTest("../Tests/test1.csv", "../Tests/test1.txt", 1);
    runTest("../Tests/test2.csv", "../Tests/test2.txt", 2);
    runTest("../Tests/test3.csv", "../Tests/test3.txt", 3);
    runTest("../Tests/test4.csv", "../Tests/test4.txt", 4);
    runTest("../Tests/test5.csv", "../Tests/test5.txt", 5);
    runTest("../Tests/test6.csv", "../Tests/test6.txt", 6);

    printf("Tests completed.\n");
    return 0;
}
