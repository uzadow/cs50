#define _XOPEN_SOURCE
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <cs50.h>
#include <stdio.h>
#include <math.h>

#define MAXCHARSOFWORD 46
#define KEYLENGTH MAXCHARSOFWORD
#define NUMBEROFDICTS 2

const string dicts[] = {"dictionaries/passwords", "dictionaries/large"};

const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//const char alphabet[] = "0123456789";
int alphaLen = (sizeof(alphabet) - sizeof(char));

char salt[3];
char key[KEYLENGTH] = {'A', '\0'};
string hash;

double logBase(double val, int base);
void recCheck();
void loopCheck();
void loopOnlyCheck();
bool dictCheck(const char *dictionary);
bool startDictAttack();
bool check();
void addToDict();
void nextChar(int index);

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

    // Executional start of dict and brute force attack:
    if (!(startDictAttack())) loopOnlyCheck();

    if (strcmp(key, "-1") == 0) printf("No valid key was found! FIX YOUR BUGS OR INPUT A CORRECT HASH!!\n");
    else printf("%s\n", key);

    return 0;
}

// Recursive aproach; too cost-intensive
void recCheck()
{
    printf("[recCheck] Starting brute force attack...\n");
    if (check()) return;
    else
    {
        nextChar(0);
        recCheck();
    }
}

// Loop aproach:
void loopCheck()
{
    printf("[loopCheck] Starting brute force attack...\n");
    while (true)
    {
        if (strcmp(key, "-1") == 0) return;
        else if (check(key, salt))
        {
            // Feeding passwords library
            addToDict();
            return;
        }
        nextChar(0);
    }
}

void loopOnlyCheck()
{
    double d = 0;

    printf("[loopOnlyCheck] Starting brute force attack...\n");
    while (true)
    {
        d++;
        for (int n = 0, m = ceil(logBase(d, alphaLen)); n < m; n++)
        {
            key[n] = alphabet[((long) (d / pow(alphaLen, n)) % (int) alphaLen)];
        }
        if (check()) return;
    }
}

// Math extension to evaluate logarithms with a different base
double logBase(double val, int base)
{
    return (log(val) / log(base));
}

// Dict attack: (Integrated are some parts of pset5's speller problem; the file handling and use of "dictionaries/large" library are to be attributed to the Harvard University)
bool dictCheck(const char *dictionary)
{
    char word[MAXCHARSOFWORD] = "\0";
    int index = 0;
    FILE *fileptr = fopen(dictionary, "r");

    printf("'%s'\n", dictionary);

    for (int c = fgetc(fileptr); c != EOF; c = fgetc(fileptr))
    {
        if (c != '\n')
        {
            word[index] = c;
            index++;
        }
        else
        {
            memset(key, 0, sizeof(key));
            strcpy(key, word);
            //printf("%s: %s\n", key, crypt(key, salt));
            if (check())
            {
                fclose(fileptr);
                return true;
            }
            memset(word, 0, sizeof(word));
            index = 0;
        }
    }
    printf("Nothing found...\n\n");
    fclose(fileptr);
    return false;
}

bool startDictAttack()
{
    for (int i = 0; i < NUMBEROFDICTS; i++)
    {
        printf("Starting dict attack using ");
        if (dictCheck(dicts[i])) return true;
    }
    return false;
}

void addToDict()
{
    FILE *fileptr = fopen(dicts[0], "a");
    fputs(key, fileptr);
    fclose(fileptr);
}

// Increments key by one element
void nextChar(int index)
{
    if (key[index] == 'z' && index >= (KEYLENGTH - 1))
    {
        strcpy(key, "-1");
        return;
    }
    else if (key[index] == 'z')
    {
        key[index] = 'A';
        nextChar(index + 1);
    }
    else if (key[index] == '\0')
    {
        key[index] = 'A';
        key[index + 1] = '\0';
    }
    else if (key[index] == 'Z')
    {
        key[index] = 'a';
    }
    else
    {
        key[index]++;
    }
}

// Check wether hash equals the hash generated from the cracked password
bool check()
{
    return (strcmp(crypt(key, salt), hash) == 0);
}
