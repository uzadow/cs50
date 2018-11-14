#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cs50.h>

#include "bmp.h"

#define MAX_SIZE 10000

int getColor (char c);

int main(int argc, char * argv[])
{
    /*======== Input validation and variable initialization ========*/
    if (argc != 4 && argc != 3)
    {
        fprintf(stderr, "Usage: %s [FILE] [Width] [~Height]\n", argv[0]);
        return 1;
    }

    char * input = "blueprint.bmp";
    char * output = argv[1];
    int outWidth;
    int outHeight;

    if (argc == 3)
    {
        outWidth = atoi (argv[2]);
        outHeight = outWidth;
    }
    else
    {
        outWidth = atoi (argv[2]);
        outHeight = atoi (argv[3]);

    }

    if (outWidth <= 0 || outHeight <= 0 || outWidth >= MAX_SIZE || outHeight >= MAX_SIZE)
    {
        fprintf(stderr, "Height and Width must be positive and smaller than %i\n", MAX_SIZE);
        return 2;
    }

    FILE * out = fopen(output, "w");
    if (out == NULL)
    {
        fprintf(stderr, "Could not open %s\n", output);
        return 3;
    }

    FILE * in = fopen(input, "r");
    if (out == NULL)
    {
        fprintf(stderr, "Blueprint file does not exist (%s)\n", input);
        return 4;
    }

    /*======== Creating new Bitmap Header ========*/
    // Read BITMAP HEADER of input file
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, in);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, in);

    // Adapting bitmap headers
    bi.biSizeImage = 4 * outWidth * outHeight;
    bf.bfSize = bi.biSizeImage + 54;

    bi.biWidth = outWidth;
    bi.biHeight = outHeight;

    printf("%i, %i\n", bi.biSizeImage, bf.bfSize);
    printf("%i, %i\n", bi.biWidth, bi.biHeight);

    // Write BITMAP HEADER of output file
    fwrite(& bf, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(& bi, sizeof(BITMAPINFOHEADER), 1, out);

    fclose(in);
    /*======== end ========*/

    // Calculate padding
    int paddout = (4 - (outWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    /*======== Take each pixel from the resized matrix and put it in the BMP ========*/
    for (int y = outHeight; y < outHeight; y++)
    {
        // iterate over pixels in scanline
        for (int x = 0; x < outWidth; x++)
        {
            printf("[%i|%i]:\n", x, y);
            RGBTRIPLE pixel = {.rgbtRed = getColor('r'), .rgbtGreen = getColor('g'), .rgbtBlue = getColor('b')};
            fwrite(& pixel, sizeof(RGBTRIPLE), 1, out);
        }

        // Write padding
        //fseek(out, paddout, SEEK_CUR);
        for (int k = 0; k < paddout; k++)
        {
            fputc(0x00, out);
        }
    }
    /*======== end ========*/

    // Close BMP files
    printf("Done\n");
    fclose(out);
    return 0;
}

int getColor (char c)
{
    int input;
    do
    {
        input = get_int ("  Color \'%c\' (0-255): ", c);
    } while (input < 0 || input > 255);
    return input;
}