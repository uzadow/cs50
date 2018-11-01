#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

//void remap (RGBTRIPLE * inmap[][], RGBTRIPLE * outmap[][], int x, int y, double n);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [0 < n < 100.0] [inputfile] [outputfile]\n]", argv[0]);
        return 1;
    }

    // n = resize factor
    double n = atof(argv[1]);
    if (n <= 0 || n > 100)
    {
        fprintf(stderr, "Resize factor n must be positive and max 100\n");
        return 5;
    }

    // Variable initialization (input and output file)
    char *input = argv[2];
    char *output = argv[3];

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

    // Make values positive
    bi.biWidth = abs(bi.biWidth);
    bi.biHeight = abs(bi.biHeight);

    // Values of input file
    int inWidth = bi.biWidth;
    int inHeight = bi.biHeight;

    bf.bfSize = floor((pow(n, 2) * (bf.bfSize - 54) + 54));
    bi.biSizeImage = floor(pow(n, 2) * bi.biSizeImage);
    // Values of output file
    bi.biWidth = floor(n * bi.biWidth);
    bi.biHeight = floor(n * bi.biHeight);

    // Write BITMAP HEADER of output file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out);

    // Calculate padding
    int paddin = (4 - (inWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddout = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Create a scanfile the size of the input BMP
    RGBTRIPLE inmap[inHeight][inWidth];
    for (int x = 0; x < inHeight; x++)
    {
        for (int y = 0; y < inWidth; y++)
        {
            // Scan each pixel of the input BMP and write it into the map
            RGBTRIPLE scan;
            fread(&scan, sizeof(RGBTRIPLE), 1, in);
            inmap[x][y] = scan;
        }
        // Skip Padding
        fseek(in, paddin, SEEK_CUR);
    }

    RGBTRIPLE outmap[bi.biWidth][bi.biHeight];
    printf("Size of output map: [%i|%i]\n", bi.biWidth, bi.biHeight);

    //remap(inmap, outmap, bi.biWidth, bi.biHeight, n);
    for (int x = 0; x < bi.biWidth; x++)
    {
        for (int y = 0; y < bi.biHeight; y++)
        {
            // scales the maps into each other
            // TODO: Check for failing conditions
            //printf("xo / yo: %i / %i;    xi / yi: %i / %i\n", x, y, (int) floor (x / n), (int) floor (y / n));
            outmap [x] [y] = inmap [(int) floor (x / n)] [(int) floor (y / n)];
        }
    }

    // iterate over infile's scanlines (over the size of the map)
    for (int x = 0; x < bi.biHeight; x++)
    {
        // iterate over pixels in scanline
        for (int y = 0; y < bi.biWidth; y++)
        {
            fwrite(&outmap[x][y], sizeof(RGBTRIPLE), 1, out);
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

/*void remap (int X, int Y, double n)
{
    // TODO Map inmap to outmap (some nice rescale algorithm?)
    for (int x = 0; x < X; x++)
    {
        for (int y = 0; y < Y; y++)
        {
            outmap [x] [y] = inmap [(int) round (x / n)] [(int) round (y / n)];
        }
    }
}*/