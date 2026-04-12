typedef enum {
    reject,
    accept,
    unknownSYMBOL,
    noTRANSITION,
    nullpointerexception,
    emptyDfa
} Result;

typedef struct {
    unsigned int from;
    char fromSymbol;
    char toSymbol;
    unsigned int to;
} Transition;

typedef struct {
    Transition* transition;
    unsigned int countTransition;
    unsigned int countAccepting;
    unsigned int* accepting;
    unsigned int start;
} Dfa;

Dfa* createDfa(Transition* transition, unsigned int* accepting,
               unsigned int countTransition, unsigned int countAccepting,
               unsigned int start);
void deleteDfa(Dfa* dfa);
Result runDfa(Dfa* dfa, char* str);
