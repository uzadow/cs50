#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

char getRanChar();

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        printf("SYNTAX: 'crypt [key]'\n");
        return 1;
    }

    char * key = argv[1];
    char salt[3] = "AA";

    salt[0] = getRanChar();
    salt[1] = getRanChar();

    printf("%s\n", crypt(key, salt));

    return 0;
}

char getRanChar()
{
    int i = round(rand() % 52);
    if (i < 26) return i + 65;
    else return i + 71;
}