#include "dfa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Transition transition[100];
    unsigned int index = 0;

    transition[index++] = (Transition) {0, '-', '-', 8};
    transition[index++] = (Transition) {0, '.', '.', 2};
    transition[index++] = (Transition) {0, '0', '9', 1};
    transition[index++] = (Transition) {8, '0', '9', 1};
    transition[index++] = (Transition) {8, '.', '.', 2};
    transition[index++] = (Transition) {1, '0', '9', 1};
    transition[index++] = (Transition) {1, '.', '.', 2};
    transition[index++] = (Transition) {1, 'E', 'E', 4};
    transition[index++] = (Transition) {2, '0', '9', 3};
    transition[index++] = (Transition) {3, '0', '9', 3};
    transition[index++] = (Transition) {3, 'E', 'E', 4};
    transition[index++] = (Transition) {4, '0', '9', 6};
    transition[index++] = (Transition) {4, '+', '+', 5};
    transition[index++] = (Transition) {4, '-', '-', 5};
    transition[index++] = (Transition) {5, '0', '9', 6};
    transition[index++] = (Transition) {6, '0', '9', 6};

    unsigned int countTransition = index;
    unsigned int accepting[] = {1, 3, 6};
    unsigned int countAccepting = 3;
    unsigned int start = 0;

    Dfa* dfa = createDfa(transition, accepting, countTransition,
                         countAccepting, start);
    if (dfa == NULL) {
        printf("Ошибка: не удалось создать автомат\n");
        return 1;
    }

    char* input = NULL;
    size_t len = 0;
    printf("Введите строку: ");
    getline(&input, &len, stdin);

    if (input == NULL) {
        deleteDfa(dfa);
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    Result result = runDfa(dfa, input);

    switch (result) {
    case accept:
        printf("Это число!\n");
        break;
    case reject:
        printf("Это не число :(\n");
        break;
    case unknownSYMBOL:
        printf("Это не число :( (неизвестный символ)\n");
        break;
    case noTRANSITION:
        printf("Это не число :( (недопустимая последовательность)\n");
        break;
    default:
        printf("Это не число :(\n");
        break;
    }

    free(input);
    deleteDfa(dfa);
    return 0;
}
