// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "trie.h"
#include "dictionary.h"

Trie *lib;
bool loaded = false;
unsigned int libSize = 0;

int getInd(char c);

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    Trie *trav = lib;
    for (int i = 0; i < strlen(word); i++)
    {
        int ind = getInd(word[i]);
        if (trav->next[ind] == NULL)
        {
            return false;
        }
        trav = trav->next[ind];
    }
    return trav->eot;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    lib = newNode();
    FILE *fileptr = fopen(dictionary, "r");
    if (!lib || !fileptr) return false;
    else
    {
        char c = 0;
        Trie *trav;
        do
        {
            libSize++;
            trav = lib;
            while(true)
            {
                c = fgetc(fileptr);
                if (c == EOF) break;
                if (c == '\n')
                {
                    trav->eot = true;
                    break;
                }
                int ind = getInd(c);
                if (trav->next[ind] == NULL) trav->next[ind] = newNode();
                trav = trav->next[ind];
            }

        }
        while (c != EOF);
        libSize--;
        return true;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return libSize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    return destroy(lib);
}

int getInd(char c)
{
    int ind = -1;
    if (c == 39) ind = 26;
    else if (c >= 'a' && c <= 'z') ind = (c - 'a');
    else if (c >= 'A' && c <= 'Z') ind = (c - 'A');
    return ind;
}