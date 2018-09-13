#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./whodunit [inputfile] [outpufil]et\n]");
        return 1;
    }

    // Variable initialization
    char *input = argv[1];
    char *output = argv[2];

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

    // Read BITMAP HEADER to confirm 24-bit BMP
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

    // Write BITMAP HEADER of output file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out);

    // Calculate padding
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            RGBTRIPLE triple;

            fread(&triple, sizeof(RGBTRIPLE), 1, in);

            if ( triple.rgbtBlue == 0xff && triple.rgbtGreen == 0xff && (triple.rgbtRed == 0xff || triple.rgbtRed == 0x00) )
            {
                triple.rgbtRed = 0xff;
            }
            else
            {
                triple.rgbtBlue = 0x00;
                triple.rgbtGreen = 0x00;
                triple.rgbtRed = 0x00;
            }
            fwrite(&triple, sizeof(RGBTRIPLE), 1, out);

        }

        // skip over padding, if any
        fseek(in, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, out);
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}