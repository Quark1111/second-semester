#include "avl_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void readFile(AvlTree* tree, char* inputfile)
{
    FILE* file = fopen(inputfile, "r");
    if (file == NULL) {
        printf("file not found\n");
        return;
    }
    char* buffer = NULL;
    size_t len = 0;
    int read;

    while ((read = getline(&buffer, &len, file)) != -1) {
        if (read > 0 && buffer[read - 1] == '\n') {
            buffer[read - 1] = '\0';
        }
        insert(tree, buffer);
    }
    free(buffer);
    fclose(file);
}

static void runInterface(AvlTree* tree, int argc, char* outputfile)
{
    char* buffer = NULL;
    size_t len = 0;

    while (true) {
        printf("> ");
        int read = getline(&buffer, &len, stdin);
        if (read == -1) {
            deleteTree(tree);
            free(buffer);
            break;
        }

        if (read > 0 && buffer[read - 1] == '\n') {
            buffer[read - 1] = '\0';
        }

        if (strncmp(buffer, "find ", 5) == 0) {
            char* code = buffer + 5;
            char* result = find(tree, code);
            if (result) {
                printf("%s -> %s\n", code, result);
            } else {
                printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
            }
        } else if (strncmp(buffer, "add ", 4) == 0) {
            char* str = buffer + 4;
            if (insert(tree, str) == 0) {
                printf("Аэропорт добавлен в базу.\n");
            } else {
                printf("Ошибка при добавлении.\n");
            }
        } else if (strncmp(buffer, "delete ", 7) == 0) {
            char* code = buffer + 7;
            int oldSize = getSize(tree);
            delete(tree, code);
            if (getSize(tree) < oldSize) {
                printf("Аэропорт '%s' удалён из базы.\n", code);
            } else {
                printf("Аэропорт с кодом '%s' не найден.\n", code);
            }
        } else if (strcmp(buffer, "save") == 0) {
            if (argc == 1) {
                char namefile[100];
                printf("Введите имя файла: ");
                if (scanf("%99s", namefile) == 1) {
                    save(tree, namefile);
                    printf("База сохранена.\n");
                }
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            } else {
                save(tree, outputfile);
                printf("База сохранена: %d аэропортов.\n", getSize(tree));
            }
        } else if (strcmp(buffer, "quit") == 0) {
            deleteTree(tree);
            free(buffer);
            return;
        } else if (strlen(buffer) > 0) {
            printf("Неизвестная команда\n");
        }

        free(buffer);
        buffer = NULL;
    }
}

int main(int argc, char* argv[])
{
    AvlTree* tree = createTree();

    if (argc == 2) {
        readFile(tree, argv[1]);
        printf("Загружено %d аэропортов. Система готова к работе.\n", getSize(tree));
        runInterface(tree, argc, argv[1]);
    } else if (argc == 1) {
        printf("Система готова к работе. База пуста.\n");
        runInterface(tree, argc, NULL);
    }
    return 0;
}
