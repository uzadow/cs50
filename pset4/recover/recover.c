#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define BLOCK 512

int fnum = 0;
char *name();

int main(int argc, char *argv[])
{
    // Check for correct input
    if (argc != 2)
    {
        fprintf(stderr, "Syntax: ./recover [input]");
        return 1;
    }

    char *infile = argv[1];
    FILE *input = fopen(infile, "r");
    if (input == NULL)
    {
        fprintf(stderr, "Could not open input file");
        return 2;
    }

    unsigned char *buffer = malloc(BLOCK);
    if (buffer == NULL)
    {
        fprintf(stderr, "Segmentation Fault");
        return 3;
    }

    FILE *tmp;

    while (fread(buffer, BLOCK, 1, input) != 0)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && buffer[3] >= 0xe0 && buffer[3] <= 0xef)
        {
            if (fnum != 0)
            {
                fclose(tmp);
            }

            tmp = fopen(name(fnum), "w");
            if (tmp == NULL)
            {
                fprintf(stderr, "Could not edit output file");
                return 4;
            }
            fnum++;
        }
        if (fnum != 0)
        {
            fwrite(buffer, BLOCK, 1, tmp);
        }
    }
    free(buffer);
    return 0;
}

char *name(num)
{
    static char tmp[12];
    sprintf(tmp, "%i%i%i.jpg", (int)floor(num / 100), (int)floor(num % 100 / 10), (int)floor(num % 10 / 1));
    return tmp;
}