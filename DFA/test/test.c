#include "dfa.h"
#include <assert.h>
#include <stdio.h>

void testCreateDfaNull()
{
    assert(createDfa(NULL, NULL, 0, 0, 0) == NULL);
    printf("OK\n");
}

void testCreateDfaValid()
{
    Transition transition[] = { { 0, 'a', 'a', 1 } };
    unsigned int accepting[] = { 1 };
    Dfa* dfa = createDfa(transition, accepting, 1, 1, 0);
    assert(dfa != NULL);
    assert(runDfa(dfa, "a") == accept);
    assert(runDfa(dfa, "b") == unknownSymbol);
    deleteDfa(dfa);
    printf("OK\n");
}

void testSimpleNumber()
{
    Transition transition[] = {
        { 0, '0', '9', 1 },
        { 1, '0', '9', 1 }
    };
    unsigned int accepting[] = { 1 };
    Dfa* dfa = createDfa(transition, accepting, 2, 1, 0);
    assert(runDfa(dfa, "123") == accept);
    deleteDfa(dfa);
    printf("OK\n");
}

void testFloatNumber()
{
    Transition transition[] = {
        { 0, '0', '9', 1 },
        { 1, '0', '9', 1 },
        { 1, '.', '.', 2 },
        { 2, '0', '9', 3 },
        { 3, '0', '9', 3 }
    };
    unsigned int accepting[] = { 1, 3 };
    Dfa* dfa = createDfa(transition, accepting, 5, 2, 0);
    assert(runDfa(dfa, "12.34") == accept);
    deleteDfa(dfa);
    printf("OK\n");
}

void testScientificNotation()
{
    Transition transition[] = {
        { 0, '0', '9', 1 },
        { 1, '0', '9', 1 },
        { 1, 'E', 'E', 4 },
        { 4, '0', '9', 6 },
        { 6, '0', '9', 6 }
    };
    unsigned int accepting[] = { 1, 6 };
    Dfa* dfa = createDfa(transition, accepting, 5, 2, 0);
    assert(runDfa(dfa, "1E10") == accept);
    deleteDfa(dfa);
    printf("OK\n");
}

void testInvalidCharacter()
{
    Transition transition[] = {
        { 0, '0', '9', 1 },
        { 1, '0', '9', 1 }
    };
    unsigned int accepting[] = { 1 };
    Dfa* dfa = createDfa(transition, accepting, 2, 1, 0);
    assert(runDfa(dfa, "12a34") == unknownSymbol);
    deleteDfa(dfa);
    printf("OK\n");
}

void testIncompleteNumber()
{
    Transition transition[] = {
        { 0, '0', '9', 1 },
        { 1, '0', '9', 1 },
        { 1, '.', '.', 2 },
        { 2, '0', '9', 3 },
        { 3, '0', '9', 3 }
    };
    unsigned int accepting[] = { 1, 3 };
    Dfa* dfa = createDfa(transition, accepting, 5, 2, 0);
    assert(runDfa(dfa, "12.") == reject);
    deleteDfa(dfa);
    printf("OK\n");
}

void testEmptyString()
{
    Transition transition[] = {
        { 0, '0', '9', 1 },
        { 1, '0', '9', 1 }
    };
    unsigned int accepting[] = { 1 };
    Dfa* dfa = createDfa(transition, accepting, 2, 1, 0);
    assert(runDfa(dfa, "") == reject);
    deleteDfa(dfa);
    printf("OK\n");
}

void testNullDfa()
{
    assert(runDfa(NULL, "test") == nullPointerError);
    printf("OK\n");
}

int main()
{
    printf("Запуск тестов ДКА:\n");
    testCreateDfaNull();
    testCreateDfaValid();
    testSimpleNumber();
    testFloatNumber();
    testScientificNotation();
    testInvalidCharacter();
    testIncompleteNumber();
    testEmptyString();
    testNullDfa();
    printf("Все тесты пройдены успешно!\n");
    return 0;
}
