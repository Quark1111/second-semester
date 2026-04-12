#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

Dfa* createDfa(Transition* transition, unsigned int* accepting, unsigned int countTransition, unsigned int countAccepting, unsigned int start)
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

static bool isAlphabet(Transition transition, char symbol)
{
    return transition.fromSymbol <= symbol && symbol <= transition.toSymbol;
}

static Result move(Dfa* dfa, unsigned int* state, char symbol)
{
    bool unknown = 1;

    for (unsigned int i = 0; i < dfa->countTransition; i++) {
        if (dfa->transition[i].from == *state
            && isAlphabet(dfa->transition[i], symbol)) {
            *state = dfa->transition[i].to;
            return accept;
        }
        if (isAlphabet(dfa->transition[i], symbol)) {
            unknown = 0;
        }
    }

    if (unknown) {
        return unknownSYMBOL;
    }
    return noTRANSITION;
}

Result runDfa(Dfa* dfa, char* str)
{
    if (dfa == NULL || dfa->accepting == NULL || dfa->transition == NULL) {
        return nullpointerexception;
    }
    if (dfa->countAccepting == 0 || dfa->countTransition == 0) {
        return emptyDfa;
    }

    unsigned int state = dfa->start;
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        Result flag = move(dfa, &state, str[i]);
        if (flag != accept) {
            return flag;
        }
    }

    for (size_t i = 0; i < dfa->countAccepting; i++) {
        if (dfa->accepting[i] == state) {
            return accept;
        }
    }
    return reject;
}

void deleteDfa(Dfa* dfa)
{
    if (dfa == NULL) {
        return;
    }
    free(dfa->transition);
    free(dfa->accepting);
    free(dfa);
}
