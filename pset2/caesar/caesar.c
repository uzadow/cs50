#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>

// Declare global variables
int key;
string plain;

int main(int argc, string argv[])
{
    // Get user input
    if (argc != 2 || argv[1] <= 0)
    {
        printf("SYNTAX: 'caesar [key]'\n");
        return 1;
    }
    key = atoi(argv[1]);
    plain = get_string("plaintext:");

    // Calculate and print ciphertext
    printf("ciphertext:");
    for (int i = 0, l = strlen(plain); i < l; i++)
    {
        if (plain[i] >= 'a' && plain[i] <= 'z')
        {
            printf("%c", (((plain[i] - 'a' + key) % 26) + 'a'));
        }
        else if (plain[i] >= 'A' && plain[i] <= 'Z')
        {
            printf("%c", (((plain[i] - 'A' + key) % 26) + 'A'));
        }
        else
        {
            printf("%c", plain[i]);
        }

    }
    printf("\n");

    return 0;
}