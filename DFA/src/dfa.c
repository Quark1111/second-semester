#include "dfa.h"
#include <stdlib.h>
#include <string.h>

struct Dfa {
    Transition* transition;
    unsigned int countTransition;
    unsigned int* accepting;
    unsigned int countAccepting;
    unsigned int start;
};

Dfa* createDfa(Transition* transition, unsigned int* accepting,
               unsigned int countTransition, unsigned int countAccepting,
               unsigned int start)
{
    if (transition == NULL || accepting == NULL) {
        return NULL;
    }
    if (countTransition == 0 || countAccepting == 0) {
        return NULL;
    }

    Dfa* dfa = malloc(sizeof(Dfa));
    if (dfa == NULL) {
        return NULL;
    }

    dfa->transition = malloc(sizeof(Transition) * countTransition);
    if (dfa->transition == NULL) {
        free(dfa);
        return NULL;
    }
    memcpy(dfa->transition, transition, sizeof(Transition) * countTransition);
    dfa->countTransition = countTransition;

    dfa->accepting = malloc(sizeof(unsigned int) * countAccepting);
    if (dfa->accepting == NULL) {
        free(dfa->transition);
        free(dfa);
        return NULL;
    }
    memcpy(dfa->accepting, accepting, sizeof(unsigned int) * countAccepting);
    dfa->countAccepting = countAccepting;

    dfa->start = start;
    return dfa;
}

static bool isSymbolInRange(Transition transition, char symbol)
{
    return transition.fromSymbol <= symbol && symbol <= transition.toSymbol;
}

static Result move(const Dfa* dfa, unsigned int* state, char symbol)
{
    bool unknown = true;

    for (unsigned int i = 0; i < dfa->countTransition; i++) {
        if (dfa->transition[i].from == *state
            && isSymbolInRange(dfa->transition[i], symbol)) {
            *state = dfa->transition[i].to;
            return accept;
        }
        if (isSymbolInRange(dfa->transition[i], symbol)) {
            unknown = false;
        }
    }

    return unknown ? unknownSymbol : noTransition;
}

Result runDfa(const Dfa* dfa, const char* str)
{
    if (dfa == NULL || dfa->accepting == NULL || dfa->transition == NULL) {
        return nullPointerError;
    }
    if (dfa->countAccepting == 0 || dfa->countTransition == 0) {
        return emptyDfa;
    }

    unsigned int state = dfa->start;
    size_t length = strlen(str);

    for (size_t i = 0; i < length; i++) {
        Result result = move(dfa, &state, str[i]);
        if (result != accept) {
            return result;
        }
    }

    for (unsigned int i = 0; i < dfa->countAccepting; i++) {
        if (dfa->accepting[i] == state) {
            return accept;
        }
    }

    return reject;
}

void deleteDfa(Dfa* dfa)
{
    if (dfa != NULL) {
        free(dfa->accepting);
        free(dfa->transition);
        free(dfa);
    }
}
