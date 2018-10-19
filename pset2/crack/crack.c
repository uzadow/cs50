#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>

#define MAXCHARSOFWORD 46
#define KEYLENGTH MAXCHARSOFWORD
#define NUMBEROFDICTS 2

string dicts[] = {"dictionaries/passwords", "dictionaries/large"};

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

char salt[3] = "AA";
char key[KEYLENGTH] = {'A', '\0'};
string hash;

void recCheck();
void loopCheck();
bool dictCheck(const char *dictionary);
void getSalt();
bool check();
void nextChar(int index);

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        printf("SYNTAX: 'crack [hash]'\n");
        return 1;
    }

    // Var initialization
    hash = argv[1];

    // Executional start of dict and brute force attack:
    if (!(startDictAttack())) loopCheck();

    if (strcmp(key, "-1") == 0) printf("No valid key was found! FIX YOUR BUGS OR INPUT A CORRECT HASH!!\n");
    else printf("%s\n", key);

    return 0;
}

// Recursive aproach; too cost-intensive
void recCheck()
{
    if (check()) return;
    else
    {
        nextChar(0);
        recCheck();
    }
}

// Loop aproach:
void loopCheck()
{
    strcpy(salt, "AA");
    while (true)
    {
        if ((strcmp(key, "-1") == 0 || check(key, hash))) return;
        nextChar(0);
    }
}

// Dict attack: (Integrated are some parts of pset5's speller problem; the file handling and use of "dictionaries/large" library are to be attributed to the Harvard University)
bool dictCheck(const char *dictionary)
{
    strcpy(salt, "AA");
    char word[MAXCHARSOFWORD] = "\0";
    int index = 0;
    FILE *fileptr = fopen(dictionary, "r");
    for (int c = fgetc(fileptr); c != EOF; c = fgetc(fileptr))
    {
        if (c != '\n')
        {
            word[index] = c;
        }
        else
        {
            strcpy(key, word);
            if (check()) return true;
            strcpy(word, "\0");
            index = 0;
        }
    }
    return false;
}

bool startDictAttack()
{
    /*for (int i = 0; i < NUMBEROFDICTS; i++)
    {
        if (dictCheck(dicts[i])) return true;
    }*/
    return false;
}

// Increments key by one element
void nextChar(int index)
{
    if (key[index] == 'z' && index >= (KEYLENGTH - 1))
    {
        strcpy(key, "-1");
        return;
    }
    else if (key[index] == 'z')
    {
        key[index] = 'A';
        nextChar(index + 1);
    }
    else if (key[index] == '\0')
    {
        key[index] = 'A';
        key[index + 1] = '\0';
    }
    else if (key[index] == 'Z')
    {
        key[index] = 'a';
    }
    else
    {
        key[index]++;
    }
}

// Check wether hash equals the hash generated from the cracked password
bool check()
{
    getSalt();
    return (strcmp(crypt(key, salt), hash) == 0);
}

// Get salt from key (first two chars)
void getSalt()
{
    if (key[1] != '\0') salt[1] = key[1];
    if (key[0] != '\0') salt[0] = key[0];
}