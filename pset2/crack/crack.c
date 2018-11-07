#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <math.h>

#define MAXCHARSOFWORD 46
#define KEY_LENGTH MAXCHARSOFWORD
#define NUM_OF_DICTS 2

const string DICTS[] = {"dictionaries/passwords", "dictionaries/large"};
const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//const char ALPHABET[] = "0123456789";
const int ALPHA_LEN = (sizeof(ALPHABET) - sizeof(char));

char salt[3];
char key[KEY_LENGTH] = {'A', '\0'};
string hash;

double logBase(double val, int base);
long long calcMaxSols(int alphaLen, int maxKeyLength);
int mapIndex(long long num, int alphaLen, int curIndex);
bool recCheck();
bool loopCheck();
bool loopOnlyCheck();
bool dictCheck(const char *dictionary);
bool startDictAttack();
bool check();
void addToDict();
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
    salt[1] = hash[1];
    salt[0] = hash[0];
    salt[2] = '\0';

    // Executional start of dict and brute force attack:
    bool found;
    if (!(startDictAttack()))
    {
        found = loopOnlyCheck();
    }

    if (strcmp(key, "-1") == 0 || !(found)) printf("No valid key was found! FIX YOUR BUGS OR INPUT A CORRECT HASH!!\n");
    else printf("%s\n", key);

    return 0;
}

// Recursive aproach; too cost-intensive
bool recCheck()
{
    printf("[recCheck] Starting brute force attack...\n");
    if (check())
    {
        return true;
    }
    else
    {
        nextChar(0);
        recCheck();
    }
    return false;
}

// Loop aproach:
bool loopCheck()
{
    printf("[loopCheck] Starting brute force attack...\n");
    // Calculate all possible Solutions for
    const long long MAX_SOLUTIONS = calcMaxSols(ALPHA_LEN, KEY_LENGTH);

    // Looping through the max possible solutions for key with max KEY_LENGTH
    for (long long i = 1; i <= MAX_SOLUTIONS; i++)
    {
        if (strcmp(key, "-1") == 0) return true;
        else if (check(key, salt))
        {
            // Feeding passwords library
            addToDict();
            return true;
        }
        nextChar(0);
    }
    return false;
}

// Loop only approach:
bool loopOnlyCheck()
{
    // Calculate all possible Solutions for
    const long long MAX_SOLUTIONS = calcMaxSols(ALPHA_LEN, KEY_LENGTH);

    // Looping through the max possible solutions for key with max KEY_LENGTH
    for (long long i = 1; i <= MAX_SOLUTIONS; i++)
    {
        // Calculate for what key length the program is checking (key length increases over time)
        int CUR_KEY_LENGTH = ceil(logBase(i, ALPHA_LEN));

        // Iterating through every char of the current key
        for (int curKeyIndex = 0; curKeyIndex < CUR_KEY_LENGTH; curKeyIndex++)
        {
            // More Math to find the correct index of the current char
            int curAlphIndex = mapIndex(i, ALPHA_LEN, curKeyIndex);

            // Sets each char of the key to the corresponding char from ALPHABET; one per iteration
            key[curKeyIndex] = ALPHABET[curAlphIndex];
        }

        // Actual test wether key is true. if so, break
        if (strcmp(crypt(key, salt), hash) == 0) return true;
    }
    return false;
}

// Math extension to evaluate logarithms with a different base
double logBase(double val, int base)
{
    return (log(val) / log(base));
}

// Equivalent to sum(KEY_LENGTH^x, 0..KEY_LENGTH)
long long calcMaxSols(int alphaLen, int maxKeyLength)
{
    return (pow(alphaLen, maxKeyLength) - alphaLen) / alphaLen - 1;
}

// Calculates what letter (index from ALPHABET) is used depending on the cur key index
int mapIndex(long long num, int alphaLen, int curIndex)
{
    return (int) (num / pow(alphaLen, curIndex)) % (int) alphaLen;
}

// Dict attack: (Integrated are some parts of pset5's speller problem; the file handling and use of "dictionaries/large" library are to be attributed to the Harvard University)
bool dictCheck(const char *dictionary)
{
    char word[MAXCHARSOFWORD] = "\0";
    int index = 0;
    FILE *fileptr = fopen(dictionary, "r");

    printf("'%s'\n", dictionary);

    for (int c = fgetc(fileptr); c != EOF; c = fgetc(fileptr))
    {
        if (c != '\n')
        {
            word[index] = c;
            index++;
        }
        else
        {
            memset(key, 0, sizeof(key));
            strcpy(key, word);
            //printf("%s: %s\n", key, crypt(key, salt));
            if (check())
            {
                fclose(fileptr);
                return true;
            }
            memset(word, 0, sizeof(word));
            index = 0;
        }
    }
    printf("Nothing found...\n\n");
    fclose(fileptr);
    return false;
}

bool startDictAttack()
{
    for (int i = 0; i < NUM_OF_DICTS; i++)
    {
        printf("Starting dict attack using ");
        if (dictCheck(DICTS[i])) return true;
    }
    return false;
}

void addToDict()
{
    FILE *fileptr = fopen(DICTS[0], "a");
    fputs(key, fileptr);
    fclose(fileptr);
}

// Increments key by one element
void nextChar(int index)
{
    if (key[index] == 'z' && index >= (KEY_LENGTH - 1))
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
    return (strcmp(crypt(key, salt), hash) == 0);
}
