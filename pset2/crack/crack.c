#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>

#define KEYLENGTH 20

string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

string recCheck(string key, string hash);
string next(string key);
string getHash(string key);
bool check(string key, string salt, string hash);
string nextChar(string s, int index);

int main(int argc, char* argv[])
{

    if (argc != 2)
    {
        printf("SYNTAX: 'crack [hash]'\n");
        return 1;
    }

    // Var initialization
    string hash = argv[1];
    char key[KEYLENGTH] = "";
    strcat(key, "A");
    strcpy(key, loopCheck(key, hash));
    if (strcmp(key, "-1") == 0) printf("No valid key was found! FIX YOUR BUGS OR INPUT A CORRECT HASH!!");
    else printf("The key is: %s", key);
}

// Recursive aproach; too cost-intensive
string recCheck(string key, string hash)
{
    if (check(getSalt(key), key, hash)) return key;
    else return recCheck(nextChar(key), hash);
}

// Loop aproach:
string loopCheck(string key, string hash)
{
    while (true)
    {
        if (check(getSalt(key), key, hash) || strcmp(key, "-1")) return key;
        else strcpy(key, nextChar(key));
    }
}

// Increments key by one element
string nextChar(string s, int index)
{
    if (s[index] == 'z')
    {
        if (index >= (KEYLENGTH - 1)) return "-1";
        else
        {
            s[index] = 'A';
            strcpy(s, nextChar(s, index + 1));
            return s;
        }
    }
    else if (s[index] == '\0')
    {
        s[index] = 'A';
        s[index + 1] = '\0';
    }
    else if (s[index] == 'Z')
    {
        s[index] = 'a';
        return s;
    }
    else
    {
        s[index]++;
        return s;
    }
}

// Check wether hash equals the hash generated from the cracked password
bool check(string key, string salt, string hash)
{
    return (strcmp(crypt(key, salt), hash) == 0);
}

// Get salt from key (first two chars)
string getSalt(string key)
{
    string tmp = "  ";
    for (int i = 0; i < 2; i++)
    {
        tmp[i] = key[i];
    }
    return tmp;
}