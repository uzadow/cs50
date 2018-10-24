#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <math.h>

#define MAXCHARSOFWORD 46
#define KEYLENGTH MAXCHARSOFWORD

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int alphaLen = (sizeof(alphabet) - sizeof(char));

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

    // Start of Brute Force:
    double d = 0;

    printf("Starting brute force attack...\n");
    ////////
    while (true)
    {
        d++;
        for (int n = 0, m = ceil(log(d) / log(alphaLen)); n < m; n++)
        {
            key[n] = alphabet[((long) ((d - 1) / pow(alphaLen, n)) % (int) alphaLen)];
        }
        if (strcmp(crypt(key, salt), hash) == 0) break;
    }
    ////////

    if (strcmp(key, "-1") == 0) printf("No valid key was found! FIX YOUR BUGS OR INPUT A CORRECT HASH!!\n");
    else printf("%s\n", key);

    return 0;
}