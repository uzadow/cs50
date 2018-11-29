// Implements a dictionary's functionality

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"

Trie *lib;
bool loaded = false;
unsigned int libSize = 0;


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Add traverser
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
    // Allocate Space for trie
    lib = newNode();

    // Open file for read
    FILE *fileptr = fopen(dictionary, "r");

    // Check wether alloc and open worked
    if (!lib || !fileptr) return false;
    // Traverse through lib-Trie and write library into it
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
        fclose(fileptr);
        return true;
    }
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Returns variable set up by load function
    return libSize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Self-explanatory
    return destroy(lib);
}

int getInd(char c)
{
    // return index for specific letter
    int ind = -1;
    if (c == 39) ind = 26;
    else if (c >= 'a' && c <= 'z') ind = (c - 'a');
    else if (c >= 'A' && c <= 'Z') ind = (c - 'A');
    return ind;
}

bool destroy(Trie *head)
{
    // Recursively delete Trie
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

Trie *newNode()
{
    // Allocate a new node and set the ptr[] to NULL
    Trie *tmp = calloc(1, sizeof(Trie));
    tmp->eot = false;

    /* For loop to initialize the pointers to the child tries with NULL - turns out not to be neccessary
    for (int i = 0; i < 27; i++)
    {
        tmp->next[i] = NULL;
    }
    */
    return tmp;
}