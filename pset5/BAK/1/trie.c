#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct tries
{
    bool eot;
    struct tries *next[27];
}
Trie;

bool destroy(Trie *head);
Trie *newNode();

Trie *newNode()
{
    Trie *tmp = malloc(sizeof(Trie));
    tmp->eot = false;
    for (int i = 0; i< 27; i++)
    {
        tmp->next[i] = NULL;
    }
    return tmp;
}

bool destroy(Trie *head)
{
    if (!head)
    {
        for (int i = 0; i < 27; i++)
        {
            if (!head->next[i]) destroy(head->next[i]);
        }
        free(head);
    }
    return 1;
}