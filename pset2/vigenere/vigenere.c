#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

string plain;
string key;

int main(int argc, string argv[])
{
    // Get user input
    if (argc != 2)
    {
        printf("SYNTAX: 'vigenere [key]'\n");
        return 1;
    }
    key = argv[1];
    for (int i = 0; i < strlen(key) - 1; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("SYNTAX: 'vigenere [key]'\n");
            return 1;
        }
    }
    plain = get_string("plaintext:");

    // Declaration and initialization of input-related variables
    int p = strlen(plain);
    int k = strlen(key);
    int ikey[k];
    int ind = 0;

    // Convert key into int array for easier processing
    for (int i = 0; i < k; i++)
    {
        if (key[i] >= 'a' && key[i] <= 'z') ikey[i] = key[i] - 'a';
        else if (key[i] >= 'A' && key[i] <= 'Z') ikey[i] = key[i] - 'A';
    }



    // Calculate and print ciphertext
    printf("ciphertext:");
    for (int i = 0; i < p; i++)
    {
        if (plain[i] >= 'a' && plain[i] <= 'z')
        {
            printf("%c",(((plain[i] - 'a') + ikey[ind % k]) % 26) + 'a');
            ind++;
        }
        else if (plain[i] >= 'A' && plain[i] <= 'Z')
        {
            printf("%c",(((plain[i] - 'A') + ikey[ind % k]) % 26) + 'A');
            ind++;
        }
        else
        {
            printf("%c",plain[i]);
        }
    }
    printf("\n");

    return 0;
}