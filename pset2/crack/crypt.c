#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>


char salt[3];
string key;
string hash;

int main(int argc, char* argv[])
{

    if (argc != 3)
    {
        printf("SYNTAX: 'crypt [hash]'\n");
        return 1;
    }

    // Var initialization
    key = argv[1];
    hash = argv[2];
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';

    printf("crypt(%s, %s): %s == %s?\n", key, salt, crypt(key, salt), hash);

    return 0;
}
