#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <math.h>

// Maximum key length to test for
#define KEY_LENGTH 10

// List of chars (add/remove if neccessary)
const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const int ALPHA_LEN = (sizeof(ALPHABET) - sizeof(char));

char salt[3];                               // Declaration of the Salt
char key[KEY_LENGTH] = {'A', '\0'};         // Initialization of the key starting with 'A'
string hash;                                // Declaration of the Hash

// Additional functions:
double logBase(double val, int base);
long long calcMaxSols(int alphaLen, int maxKeyLength);
int mapIndex(long long num, int alphaLen, int curIndex);

int main(int argc, char* argv[])
{
    // Check for syntax
    if (argc != 2)
    {
        printf("SYNTAX: '%s [hash]'\n", argv[0]);
        return 1;
    }

    hash = argv[1];                         // Get Hash from call arguments
    salt[0] = hash[0];                      // Recreate Salt from the Hash
    salt[1] = hash[1];
    salt[2] = '\0';

    printf("Starting brute force attack...\n");
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
        if (strcmp(crypt(key, salt), hash) == 0) break;
    }

    if (strcmp(key, "-1") == 0) printf("No valid key was found - Maybe fix kill some bugs...?\n");
    else printf("Key: %s\n", key);

    return 0;
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