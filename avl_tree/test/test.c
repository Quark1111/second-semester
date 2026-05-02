#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void testCreateTree()
{
    AvlTree* tree = createTree();
    assert(tree != NULL);
    assert(find(tree, "test") == NULL);
    deleteTree(tree);
    printf("testCreateTree: OK\n");
}

void testInsertAndFind()
{
    AvlTree* tree = createTree();
    assert(insert(tree, "LED:Pulkovo") == 0);
    assert(insert(tree, "SVO:Sheremetyevo") == 0);
    assert(insert(tree, "DME:Domodedovo") == 0);

    assert(strcmp(find(tree, "LED"), "Pulkovo") == 0);
    assert(strcmp(find(tree, "SVO"), "Sheremetyevo") == 0);
    assert(strcmp(find(tree, "DME"), "Domodedovo") == 0);
    assert(find(tree, "XXX") == NULL);

    deleteTree(tree);
    printf("testInsertAndFind: OK\n");
}

void testDuplicateInsert()
{
    AvlTree* tree = createTree();
    assert(insert(tree, "LED:Pulkovo") == 0);
    assert(insert(tree, "LED:Pulkovo") == -1);

    deleteTree(tree);
    printf("testDuplicateInsert: OK\n");
}

void testDelete()
{
    AvlTree* tree = createTree();
    insert(tree, "LED:Pulkovo");
    insert(tree, "SVO:Sheremetyevo");
    insert(tree, "DME:Domodedovo");

    delete(tree, "SVO");
    assert(find(tree, "SVO") == NULL);
    assert(find(tree, "LED") != NULL);
    assert(find(tree, "DME") != NULL);

    deleteTree(tree);
    printf("testDelete: OK\n");
}

void testDeleteNonExistent()
{
    AvlTree* tree = createTree();
    insert(tree, "LED:Pulkovo");

    delete(tree, "ffdsfsdfsdfsdfsdfsfsdfsfsd");
    assert(find(tree, "LED") != NULL);

    deleteTree(tree);
    printf("testDeleteNonExistent: OK\n");
}

void testFindInEmptyTree()
{
    AvlTree* tree = createTree();
    assert(find(tree, "LED") == NULL);
    assert(find(tree, "") == NULL);
    deleteTree(tree);
    printf("testFindInEmptyTree: OK\n");
}

void testDeleteAll()
{
    AvlTree* tree = createTree();
    insert(tree, "A:1");
    insert(tree, "B:2");
    insert(tree, "C:3");
    insert(tree, "D:4");
    insert(tree, "E:5");

    delete(tree, "A");
    delete(tree, "B");
    delete(tree, "C");
    delete(tree, "D");
    delete(tree, "E");

    assert(find(tree, "A") == NULL);
    assert(find(tree, "B") == NULL);
    assert(find(tree, "C") == NULL);
    assert(find(tree, "D") == NULL);
    assert(find(tree, "E") == NULL);

    deleteTree(tree);
    printf("testDeleteAll: OK\n");
}

void testInsertError()
{
    AvlTree* tree = createTree();
    assert(insert(tree, "LEDPulkovo") == -1);
    assert(insert(NULL, "LED:Pulkovo") == -1);

    deleteTree(tree);
    printf("testInsertError: OK\n");
}

int main()
{
    printf("Running AVL tree tests:\n");

    testCreateTree();
    testInsertAndFind();
    testDuplicateInsert();
    testDelete();
    testDeleteNonExistent();
    testFindInEmptyTree();
    testDeleteAll();
    testInsertError();
    
    printf("All tests passed!\n");
    return 0;
}
