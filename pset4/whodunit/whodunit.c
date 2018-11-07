#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s [0 < n < 100.0] [inputfile] [outputfile]\n]", argv[0]);
        return 1;
    }

    // Variable initialization (input and output file)
    char *input = argv[1];
    char *output = argv[2];

    // Open files
    FILE *in = fopen(input, "r");
    if (in == NULL)
    {
        fprintf(stderr, "Could not read file");
        return 2;
    }
    FILE *out = fopen(output, "w");
    if (out == NULL)
    {
        fprintf(stderr, "Could not write to file");
        fclose(in);
        return 3;
    }

    // Read BITMAP HEADER of input file
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, in);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, in);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fprintf(stderr, "Could not recognize format.\n");
        fclose(out);
        fclose(in);
        return 4;
    }

    // Make values positive
    bi.biWidth = abs(bi.biWidth);
    bi.biHeight = abs(bi.biHeight);

    // Write BITMAP HEADER of output file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out);

    // Calculate padding
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Create a scanfile the size of the input BMP
    for (int x = 0; x < bi.biWidth; x++)
    {
        for (int y = 0; y < bi.biHeight; y++)
        {
            // Scan each pixel of the input BMP and write it into the map
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, in);

            // make text black, everything else grey
            if (!(triple.rgbtBlue >= 0xf1 && triple.rgbtRed >= 0xf1 && triple.rgbtGreen >= 0xf1))
            {
                triple.rgbtBlue = 0x00;
                triple.rgbtGreen = 0x00;
                triple.rgbtRed = 0x00;
            }
            else    // if not red make black
            {
                triple.rgbtBlue = 0xff;
                triple.rgbtGreen = 0xff;
                triple.rgbtRed = 0xff;
            }
            fwrite(&triple, sizeof(RGBTRIPLE), 1, out);
        }

        // Skip over padding, if there is
        fseek(in, padding, SEEK_CUR);
    }

    fclose(in);
    fclose(out);
    return 0;
}