#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>


string salt;
string key;
string hash;

int main(int argc, char* argv[])
{

    if (argc != 4)
    {
        printf("SYNTAX: 'crack [hash]'\n");
        return 1;
    }

    // Var initialization
    key = argv[1];
    salt = argv[2];
    hash = argv[3];

    printf("crypt(%s, %s): %s == %s?\n", key, salt, crypt(key, salt), hash);

    return 0;
}
