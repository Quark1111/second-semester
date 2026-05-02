#pragma once

#include <stdbool.h>

typedef struct {
    unsigned int from;
    char fromSymbol;
    char toSymbol;
    unsigned int to;
} Transition;

typedef enum {
    accept,
    reject,
    unknownSymbol,
    noTransition,
    nullPointerError,
    emptyDfa
} Result;

typedef struct Dfa Dfa;

// Создаёт детерминированный конечный автомат.
Dfa* createDfa(Transition* transition, unsigned int* accepting,
    unsigned int countTransition, unsigned int countAccepting,
    unsigned int start);

// Проверяет, принадлежит ли строка языку автомата.
Result runDfa(const Dfa* dfa, const char* str);

// Удаляет автомат и освобождает память.
void deleteDfa(Dfa* dfa);
