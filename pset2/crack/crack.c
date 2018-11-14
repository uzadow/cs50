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
    strcpy(key, recCheck(key, hash));
    if (strcmp(key, "-1") == 0) printf("No valid key was found! FIX YOUR BUGS OR INPUT A CORRECT HASH!!");
    else printf("The key is: %s", recCheck(key, hash));
}

string recCheck(string key, string hash)
{
    if (check(getHash(key), key, hash)) return key;
    else return recCheck(next(key), hash);
}

string next(string key)
{
    key = nextChar(key, 0);
    return key;
}

string nextChar(string s, int index)
{
    if (s[index] == 'z')
    {
        if (index >= (KEYLENGTH - 1)) return "-1";
        else
        {
            s[index] = 'A';
            return nextChar(s, index + 1);
        }
    }
    else if (s[index] == 'Z')
    {
        s[index] = 'a';
    }
    else
    {
        s[index]++;
    }
    return s;
}

bool check(string key, string salt, string hash)
{
    return (strcmp(crypt(key, salt), hash) == 0);
}

string getHash(string key)
{
    string tmp = "  ";
    for (int i = 0; i < 2; i++)
    {
        tmp[i] = key[i];
    }
    return tmp;
}