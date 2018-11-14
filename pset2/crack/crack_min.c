#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <math.h>

#define KEYLENGTH 46

const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int ALPHA_LEN = (sizeof(ALPHABET) - sizeof(char));

char salt[3];
char key[KEYLENGTH] = {'A', '\0'};
string hash;

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

    printf("Starting brute force attack...\n");
    // Looping through the max possible solutions for key with max KEYLENGTH
    for (double d = 1, b = ((pow(ALPHA_LEN, KEYLENGTH) - ALPHA_LEN) / ALPHA_LEN - 1); d <= b; d++)
    {
        // Iterating through every char of the current key
        for (int n = 0, m = ceil(log(d) / log(ALPHA_LEN)); n < m; n++)
        {
            // More Math to find the correct index of the current char
            key[n] = ALPHABET[((int) (d / pow(ALPHA_LEN, n)) % (int) ALPHA_LEN)];
        }
        if (strcmp(crypt(key, salt), hash) == 0) break;
    }

    if (strcmp(key, "-1") == 0) printf("No valid key was found - Maybe fix kill some bugs...?\n");
    else printf("%s\n", key);

    return 0;
}