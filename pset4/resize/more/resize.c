#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

typedef struct
{
    int r;
    int g;
    int b;
} SumCol;

/*========- Functions -========*/
// Refactors inputMatrix to outputMatrix through rounding. Deprecated.
void refactor (RGBTRIPLE * inputMatrix [], RGBTRIPLE * outputMatrix [], int X, int Y, double n);

// Resizing algorithm using approximations
void resize (RGBTRIPLE * inputMatrix [], RGBTRIPLE * outputMatrix [], int X, int Y, double n);

// Averages the color in a field in a RGBTRIPLE matrix
void average (RGBTRIPLE * pixel, RGBTRIPLE * inputMatrix [], double lowX, double lowY, double highX, double highY);

// Sums up all colors in a field in a RGBTRIPLE matrix
void sumColors (SumCol * sum, RGBTRIPLE * inputMatrix [], double lowX, double lowY, double highX, double highY);

// Gets called by resize(); calculates the average color for each pixel of the output Matrix
void calcPixel (RGBTRIPLE * pixel, RGBTRIPLE * inputMatrix [], int X, int Y, double n);

// Calculates the amount of pixels in a field in a matrix
int calcPixelAmount (double lowerBound, double upperBound, double lowerArea, double upperArea);

// adds the specific percentage of the pixel color value to the color sum
void addHexPercentage (SumCol * sum, RGBTRIPLE pixel, double percentage);
/*========- Functions -========*/

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

    // Recalculate resize factor#
    n = (double) bi.biWidth / inWidth;

    // Calculate padding
    int paddin = (4 - (inWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddout = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Create a scanfile the size of the input BMP (manually allocating mem)
    RGBTRIPLE * inputMatrix [inWidth];
    for (int i = 0; i < inWidth; i++) { inputMatrix [i] = malloc (inHeight * sizeof (RGBTRIPLE)); }

    for (int x = 0; x < inHeight; x++)
    {
        for (int y = 0; y < inWidth; y++)
        {
            // Scan each pixel of the input BMP and write it into the map
            RGBTRIPLE scan;
            fread(&scan, sizeof(RGBTRIPLE), 1, in);
            inputMatrix[x][y] = scan;
        }
        // Skip Padding
        fseek(in, paddin, SEEK_CUR);
    }

    RGBTRIPLE * outputMatrix [bi.biWidth];
    for (int i = 0; i < bi.biWidth; i++) { outputMatrix [i] = malloc (bi.biHeight * sizeof (RGBTRIPLE)); }
    printf("Size of output map: [%i|%i]\n", bi.biWidth, bi.biHeight);
    // resize or refactor
    resize (inputMatrix, outputMatrix, bi.biWidth, bi.biHeight, n);

    // iterate over infile's scanlines (over the size of the map)
    for (int x = 0; x < bi.biHeight; x++)
    {
        // iterate over pixels in scanline
        for (int y = 0; y < bi.biWidth; y++)
        {
            fwrite(&outputMatrix[x][y], sizeof(RGBTRIPLE), 1, out);
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

void refactor (RGBTRIPLE * inputMatrix [], RGBTRIPLE * outputMatrix [], int outWidth, int outHeight, double n)
{
    for (int y = 0; y < outHeight; y++)
    {
        for (int x = 0; x < outWidth; x++)
        {
            // scales the maps into each other
            // TODO: Check for failing conditions
            //printf("xo / yo: %i / %i;    xi / yi: %i / %i\n", x, y, (int) floor (x / n), (int) floor (y / n));
            outputMatrix [x][y] = inputMatrix [(int) floor (x / n)] [(int) floor (y / n)];
        }
    }
}

// Resizing algorithm:
void resize (RGBTRIPLE * inputMatrix [], RGBTRIPLE * outputMatrix [], int outWidth, int outHeight, double n)
{
    printf("%i / %i\n", outWidth, outHeight);
    for (int x = 0; x < outWidth; x++)
    {
        for (int y = 0; y < outHeight; y++)
        {
            printf("(%i / %i ): ", x, y);
            calcPixel (& outputMatrix [x][y], inputMatrix, x, y, n);
        }
    }
}

void calcPixel (RGBTRIPLE * pixel, RGBTRIPLE * inputMatrix [], int X, int Y, double n)
{
    // Calc the coordinates of the area in the input Matrix
    average (pixel, inputMatrix, X / n, Y / n, (X + 1) / n, (Y + 1) / n);
}

// Calc the average color in the given area
void average (RGBTRIPLE * pixel, RGBTRIPLE * inputMatrix [], double lowX, double lowY, double highX, double highY)
{
    // Check wether given area only contains one pixel
    if ((ceil (highX) - floor (lowX) == 1) && (ceil (highY) - floor (lowY) == 1))
    {
        printf("\n");
        pixel->rgbtRed = inputMatrix [(int) floor (lowX)] [(int) floor (lowY)].rgbtRed;
        pixel->rgbtBlue = inputMatrix [(int) floor (lowX)] [(int) floor (lowY)].rgbtBlue;
        pixel->rgbtGreen = inputMatrix [(int) floor (lowX)] [(int) floor (lowY)].rgbtGreen;
        return;
    }

    // Calculate area
    double area = fabs (highX - lowX) * fabs (highY - lowY);

    // Create struct to store the summed up color vals
    SumCol * sum = malloc (sizeof(SumCol)); // Total sum of the color values of all pixels in a field, taking percentage into account
    sum->r = 0;
    sum->g = 0;
    sum->b = 0;

    // Calculate sum of hex colors
    sumColors (sum, inputMatrix, lowX, lowY, highX, highY);

    // Average sum and area and put it into the matrix
    pixel->rgbtRed = (BYTE) sum->r / area;
    pixel->rgbtBlue = (BYTE) sum->b / area;
    pixel->rgbtGreen = (BYTE) sum->g / area;
    printf("\n");

    free(sum);
}

void sumColors (SumCol * sum, RGBTRIPLE * inputMatrix [], double lowX, double lowY, double highX, double highY)
{
    // Calculate the margin
    double dx1, dy1, dx2, dy2;
    dx1 = ceil (lowX) - lowX;
    dy1 = ceil (lowY) - lowY;
    dx2 = fabs (floor (highX) - highX);
    dy2 = fabs (floor (highY) - highY);

    // Calculate the starting coords of the first pixel (for easier iterations)
    int X0 = floor (lowX);
    int Y0 = floor (lowY);

    // Calculate the amount of pixels and pixelfragments
    int lenX = calcPixelAmount (lowX, highX, dx1, dx2);
    int lenY = calcPixelAmount (lowY, highY, dy1, dy2);
    printf("lenX: %i; lenY: %i; ", lenX, lenY);

    // Iterate over every pixel in the given area and add the percentage of the hex value to the color sum
    for (int y = 0; y < lenY; y++)
    {
        for (int x = 0; x < lenX; x++)
        {
            double tdx = 1;
            if (x == 0) tdx = dx1;
            else if (x == lenX - 1) tdx = dx2;

            double tdy = 1;
            if (y == 0) tdy = dy1;
            else if (y == lenY - 1) tdy = dy2;

            addHexPercentage (sum, inputMatrix [X0 + x][Y0 + y], tdx * tdy);
        }
    }
}


int calcPixelAmount (double lowerBound, double upperBound, double lowerArea, double upperArea)
{
    return (int) ((floor (upperBound) - ceil (lowerBound)) + (ceil (lowerArea) - floor (lowerArea)) + (ceil (upperArea) - floor (upperArea)));
}

void addHexPercentage (SumCol * sum, RGBTRIPLE pixel, double percentage)
{
    sum->r += (int) pixel.rgbtRed * percentage;
    sum->b += (int) pixel.rgbtBlue * percentage;
    sum->g += (int) pixel.rgbtGreen * percentage;
}