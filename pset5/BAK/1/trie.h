#ifndef TRIE_H_
#define TRIE_H_

#include <stdbool.h>

typedef struct tries
{
    bool eot;
    struct tries *next[27];
}
Trie;

bool destroy(Trie *head);
Trie *newNode();

#endif // TRIE_H_