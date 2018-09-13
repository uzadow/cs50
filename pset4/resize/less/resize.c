#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize [0 < n < 101] [inputfile] [outpufil]et\n]");
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <=0 || n > 100)
    {
        fprintf(stderr, "Resize factor n must be positive and max 100\n");
        return 5;
    }

    // Variable initialization
    char *input = argv[2];
    char *output = argv[3];

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

    int inWidth = bi.biWidth;
    int inHeight = bi.biHeight;

    bf.bfSize = (pow(n, 2) * (bf.bfSize - 54) + 54);
    bi.biSizeImage = pow(n, 2) * bi.biSizeImage;
    bi.biWidth = n * bi.biWidth;
    bi.biHeight = n * bi.biHeight;

    // Write BITMAP HEADER of output file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out);

    // Calculate padding
    int paddin = (4 - (inWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddout = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    inWidth -= paddin;

    // Create a scanfile
    RGBTRIPLE map[abs(bi.biHeight)][bi.biWidth];
    for (int i = 0, biHeight = abs(inHeight); i < biHeight; i++)
    {
        for (int j = 0; j < inWidth; j++)
        {
            RGBTRIPLE scan;
            fread(&scan, sizeof(RGBTRIPLE), 1, in);
            for (int a = 0; a < n; a++)
            {
                for (int b = 0; b < n; b++)
                {
                    map[n * i + a][n * j + b] = scan;
                }
            }
        }
        fseek(in, paddin, SEEK_CUR);
        for (int k = 0; k < paddin; k++)
        {
            RGBTRIPLE triple;
            fread(&triple, sizeof(RGBTRIPLE), 1, in);
        }
    }


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            fwrite(&map[i][j], sizeof(RGBTRIPLE), 1, out);
        }

        fseek(in, paddout, SEEK_CUR);
        for (int k = 0; k < paddout; k++)
        {
            fputc(0x00, out);
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}