#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bmp.h"

#define MAX_FACTOR 10000.0

typedef struct
{
    double r;
    double g;
    double b;
} Color;

typedef struct
{
    double x1, y1;
    double x2, y2;
} Area;

/*======== Function definitions ========*/
// Implementation of averaging resizing algorithm
void resize (RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int outWidth, int outHeight, double factor);

// Calculates the average value for each pixel in the output BMP
RGBTRIPLE calcPixel (RGBTRIPLE * inMatrix [], Area area);
/*=====================================*/

int main(int argc, char *argv[])
{
    /*======== Input validation and variable initialization ========*/
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [0 < new Size < %f] [input BMP] [output BMP]\n", argv[0], MAX_FACTOR);
        return 1;
    }

    // n = resize factor
    int newSize = atoi(argv[1]);
    if (newSize <= 0 || newSize > MAX_FACTOR)
    {
        fprintf(stderr, "New Size n must be positive and max %f\n", MAX_FACTOR);
        return 5;
    }

    // Variable initialization (input and output file)
    char * input = argv[2];
    char * output = argv[3];

    // Open files
    FILE * in = fopen(input, "r");
    if (in == NULL)
    {
        fprintf(stderr, "Could not read file");
        return 2;
    }
    FILE * out = fopen(output, "w");
    if (out == NULL)
    {
        fprintf(stderr, "Could not write to file");
        fclose(in);
        return 3;
    }

    /*======== Creating new Bitmap Header ========*/
    // Read BITMAP HEADER of input file
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, in);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, in);

    // Recalculate resize factor
    double factor = fabs ((double) newSize / bi.biWidth);

    // Values of input file
    int inWidth = abs (bi.biWidth);
    int inHeight = abs (bi.biHeight);

    // Values of output file
    int outWidth = (int) (factor * inWidth);
    int outHeight = (int) (factor * inHeight);

    // Adapting bitmap headers
    bi.biSizeImage = (int) (pow(factor, 2) * bi.biSizeImage);
    bf.bfSize = bi.biSizeImage + 54;

    bi.biWidth = outWidth;
    bi.biHeight = outHeight;

    // Write BITMAP HEADER of output file
    fwrite(& bf, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(& bi, sizeof(BITMAPINFOHEADER), 1, out);
    /*======== end ========*/

    // Calculate padding
    int paddin = (4 - (inWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddout = (4 - (outWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    /*======== Take each pixel from the input BMP and store it in a matrix ========*/
    RGBTRIPLE * inputMatrix [inWidth];
    for (int i = 0; i < inWidth; i++) { inputMatrix [i] = malloc (inHeight * sizeof (RGBTRIPLE)); }

    for (int x = 0; x < inWidth; x++)
    {
        for (int y = 0; y < inHeight; y++)
        {
            // Scan each pixel of the input BMP and write it into the map
            RGBTRIPLE scan;
            fread(&scan, sizeof(RGBTRIPLE), 1, in);
            inputMatrix [y][x] = scan;
        }
        // Skip Padding
        fseek(in, paddin, SEEK_CUR);
    }
    /*======== end ========*/

    // Create second RGBTRIPLE matrix for the output BMP
    RGBTRIPLE * outputMatrix [outHeight];
    for (int i = 0; i < outHeight; i++) { outputMatrix [i] = malloc (outWidth * sizeof (RGBTRIPLE)); }

    printf("Resize from [%i|%i] to [%i|%i].\n", inWidth, inHeight, outWidth, outHeight);

    // RESIZING ALGORITHM
    resize (inputMatrix, outputMatrix, outWidth, outHeight, factor);

    /*======== Take each pixel from the resized matrix and put it in the BMP ========*/
    for (int y = 0; y < outHeight; y++)
    {
        // iterate over pixels in scanline
        for (int x = 0; x < outWidth; x++)
        {
            fwrite(& outputMatrix[y][x], sizeof(RGBTRIPLE), 1, out);
        }
        fseek(in, paddout, SEEK_CUR);
        for (int k = 0; k < paddout; k++)
        {
            fputc(0x00, out);
        }
    }
    /*======== end ========*/

    // Close BMP files
    fclose(in);
    fclose(out);
    return 0;
}

void resize (RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int outWidth, int outHeight, double factor)
{
    // Iterate over every pixel of the output Matrix
    for (int x = 0; x < outWidth; x++)
    {
        for (int y = 0; y < outHeight; y++)
        {
            printf("(%i|%i): ", x, y);
            outMatrix [y][x] = calcPixel (inMatrix, getArea (x, y, factor));
            printf("\n");
        }
    }
}

// Done
Area getArea (int x, int y, double factor)
{
    Area area
    {
        .x1 = x / factor,
        .x2 = (x + 1) / factor,
        .y1 = y / factor,
        .y2 = (y + 1) / factor
    };
    return area;
}

// Done
RGBTRIPLE calcPixel (RGBTRIPLE * inMatrix [], Area area)
{
    Color color = {.r = 0, .g = 0, .b = 0};
    sumUpColor (& color, inMatrix, area);
    return getPixelFromColor (divColorByArea (color, area));
}

// TODO: iterate over pixel + add color
void sumUpColor (Color * color, RGBTRIPLE * inMatrix [], Area area)
{

}

// Done
Color divColorByArea (Color colorSum, Area area)
{
    double percentage (calcArea (area));
    Color color =
    {
        .r = colorSum.r / percentage,
        .g = colorSum.g / percentage,
        .b = colorSum.b / percentage
    }
    return color;
}
// Done
double calcArea (Area area)
{
    return ((area.x2 - area.x1) * (area.y2 - area.y1));
}