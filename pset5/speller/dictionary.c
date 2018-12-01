// Implements a dictionary's functionality

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"

Trie *dict;
bool loaded = false;
unsigned int dictSize = 0;
unsigned int firstFreeNode;


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Initialize traverser
    Trie *trav = dict;

    // Go through every char of the given word. Return wether last char is the end of a word
    int i=0;
    while(word[i] != 0) 
    {
        int ind = getInd(word[i]);
        trav = trav->next[ind];
        if (trav == NULL)
        {
            return false;
        }
        i++;
    }
    return trav->eot;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Allocate dict memory and initialize first node.
    dict = calloc(1000000, sizeof(Trie));
    firstFreeNode = 0;
    newNode();

    FILE *fileptr = fopen(dictionary, "r");

    // Check wether alloc and open worked
    if (!dict || !fileptr) return false;

    // Traverse through dict-Trie and write dictionary into it
    char c;
    Trie *trav;
    do
    {
        dictSize++;  // Update size of words
        trav = dict; // Declare temp. traverser

        // Iterate through every char
        for (c = fgetc (fileptr); c != EOF; c = fgetc (fileptr))
        {
            // Break if end of word
            if (c == '\n')
            {
                trav->eot = true;
                break;
            }

            // Otherwise go to next Trie at index of the char
            int ind = getInd(c);
            if (trav->next[ind] == NULL) trav->next[ind] = newNode();
            trav = trav->next[ind];
        }

    }
    while (c != EOF);

    dictSize--;      // Fix off-by-one error
    fclose(fileptr);
    loaded = true;
    printf("Number of trie nodes: %i\n", firstFreeNode);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Returns variable set up by load function
    if (loaded) return dictSize;
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free(dict);
    return true;
}

int getInd(char c)
{
    // return index for specific letter (a-z -> 0-25; ' -> 26, FAIL -> -1)
    if (c >= 'a' && c <= 'z') return (c - 'a');
    if (c >= 'A' && c <= 'Z') return (c - 'A');
    if (c == 39) return 26;
    return -1;
}

Trie *newNode()
{
    // Allocate a new node and set the ptr[] to NULL
    Trie *tmp = &dict[firstFreeNode];
    tmp->eot = false;
    firstFreeNode++;
    return tmp;
}
