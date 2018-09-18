// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdlib.h>
#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// Prototypes
bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);


typedef struct tries
{
    bool eot;
    struct tries *next[27];
}
Trie;


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



#endif // DICTIONARY_H
