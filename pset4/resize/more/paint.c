#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cs50.h>

#include "bmp.h"

#define MAX_SIZE 10000

int getColor (char c);

RGBTRIPLE getPixel();

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

    /*bool getHex;
    char tmp;
    do
    {
        tmp = get_char ("Wanna use Hex values? (Y/n) ");
        if (tmp == 'y') getHex = true;
        if (tmp == 'n') getHex = false;
    } while (tmp != 'y' && tmp != 'n');
*/


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
            RGBTRIPLE pixel = getPixel();
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

RGBTRIPLE getPixel ()
{
    char color;
    RGBTRIPLE pixel;
    do
    {
        color = get_char ("Input Color: ");
    } while (color < 'a' || color > 'z');
    switch (color)
    {
        case 'r':
            pixel.rgbtRed = 255;
            pixel.rgbtGreen = 0;
            pixel.rgbtBlue = 0;
            break;
        case 'g':
            pixel.rgbtRed = 0;
            pixel.rgbtGreen = 255;
            pixel.rgbtBlue = 0;
            break;
        case 'b':
            pixel.rgbtRed = 0;
            pixel.rgbtGreen = 0;
            pixel.rgbtBlue = 255;
            break;
        case 'c':
            pixel.rgbtRed = 0;
            pixel.rgbtGreen = 255;
            pixel.rgbtBlue = 255;
            break;
        case 'y':
            pixel.rgbtRed = 255;
            pixel.rgbtGreen = 255;
            pixel.rgbtBlue = 0;
            break;
        case 'm':
            pixel.rgbtRed = 255;
            pixel.rgbtGreen = 0;
            pixel.rgbtBlue = 255;
            break;
        case 'o':
            pixel.rgbtRed = 255;
            pixel.rgbtGreen = 127;
            pixel.rgbtBlue = 0;
            break;
        case 'p':
            pixel.rgbtRed = 127;
            pixel.rgbtGreen = 0;
            pixel.rgbtBlue = 127;
            break;
        case 'l':
            pixel.rgbtRed = 127;
            pixel.rgbtGreen = 255;
            pixel.rgbtBlue = 0;
            break;
        case 't':
            pixel.rgbtRed = 0;
            pixel.rgbtGreen = 127;
            pixel.rgbtBlue = 127;
            break;
        case 'd':
            pixel.rgbtRed = 0;
            pixel.rgbtGreen = 0;
            pixel.rgbtBlue = 0;
            break;
        case 'w':
            pixel.rgbtRed = 255;
            pixel.rgbtGreen = 255;
            pixel.rgbtBlue = 255;
            break;
        default :
            pixel.rgbtRed = 127;
            pixel.rgbtGreen = 127;
            pixel.rgbtBlue = 127;
    }
    return pixel;
}