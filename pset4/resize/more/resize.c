#include <stdbool.h>
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

// Creates an initialized Area struct
Area getArea (int x, int y, double factor);

// Calculates the average value for each pixel in the output BMP
RGBTRIPLE calcPixel (RGBTRIPLE * inMatrix [], Area area);

// Adds the colors (margin + contend) from all pixels in the field up in a Color struct
void sumUpColor (Color * color, RGBTRIPLE * inMatrix [], Area area);

// Adds all whole pixels in the field to the Color struct
void sumUpWholePixels (Color * color, RGBTRIPLE * inMatrix [], Area area);

// Checks wether field only contains one pixel
bool fieldInPixel (Area area);

// Checks wether margin's smaller than one column of pixels
bool sideInPixel (double lowerVal, double upperVal);

// Iterates over each pixel in the margin and calls addMargin
void sumUpMargin (Color * color, RGBTRIPLE * inMatrix [], Area area);

// Adds a pixel from the margin to the Color sum
void addMargin (Color * color, RGBTRIPLE * inMatrix [], double x, double y, double percentage);

// Adds a stripe (smaller but longer than one pixel) to the Color sum
void addStripe (Color * color, RGBTRIPLE * inMatrix [], Area area, char dir);

// Adds the RGB value of given pixel to given Color struct
void addPixelToColor (Color * color, RGBTRIPLE pixel, double percentage);

// Divides given color by given area and returns it in a new Color struct
Color divColorByArea (Color colorSum, Area area);

// Converts a Color to an RGBTRIPLE
RGBTRIPLE getPixelFromColor (Color color);

// Calculates the size of an area
double calcArea (Area area);
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
    Area area =
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

// Done
void sumUpColor (Color * color, RGBTRIPLE * inMatrix [], Area area)
{
    if (fieldInPixel (area))
    {
        addMargin (color, inMatrix, area.x1, area.y1, calcArea (area));
    }
    else if (sideInPixel(area.x1, area.x2))
    {
        addStripe (color, inMatrix, area, 'x');
    }
    else if (sideInPixel(area.y1, area.y2))
    {
        addStripe (color, inMatrix, area, 'y');
    }
    else
    {
        sumUpWholePixels (color, inMatrix, area);
        sumUpMargin (color, inMatrix, area);
    }
}

// Done
bool fieldInPixel (Area area)
{
    return (sideInPixel (area.x1, area.x2) && sideInPixel (area.y1, area.y2));
}

// Done
bool sideInPixel (double lowerVal, double upperVal)
{
    return (ceil (upperVal) - floor (lowerVal) == 1);
}

// Done
void sumUpWholePixels (Color * color, RGBTRIPLE * inMatrix [], Area area)
{
    for (int y = (int) ceil (area.y1); y < floor (area.y2); y++)
    {
        for (int x = (int) ceil (area.x1); x < floor (area.x2); x++)
        {
            addPixelToColor (color, inMatrix [x][y], 1.0);
        }
    }
}

// Done
void sumUpMargin (Color * color, RGBTRIPLE * inMatrix [], Area area)
{
    double left = ceil (area.x1) - area.x1;
    double right = area.x2 - floor (area.x2);
    double top = ceil (area.y1) - area.y1;
    double bottom = area.y2 - floor (area.y2);

    // Add corner pixels
    addMargin (color, inMatrix, area.x1, area.y1, left * top);
    addMargin (color, inMatrix, area.x2, area.y1, top * right);
    addMargin (color, inMatrix, area.x1, area.y2, right * bottom);
    addMargin (color, inMatrix, area.x2, area.y2, bottom * left);

    // Add left and right margin
    for (double y = ceil (area.y1); y < floor (area.y2); y++)
    {
        addMargin (color, inMatrix, area.x1, y, left);
        addMargin (color, inMatrix, area.x2, y, right);
    }

    // Add top and bottom margin
    for (double x = ceil (area.x1); x < floor (area.x2); x++)
    {
        addMargin (color, inMatrix, x, area.y1, top);
        addMargin (color, inMatrix, x, area.y2, bottom);
    }
}

// Done
void addMargin (Color * color, RGBTRIPLE * inMatrix [], double x, double y, double percentage)
{
    addPixelToColor (color, inMatrix [(int) floor (x)][(int) floor (y)], percentage);
}

// Done
void addStripe (Color * color, RGBTRIPLE * inMatrix [], Area area, char dir)
{
    double left, right, top, bottom;

    // For stripes with height smaller than one but longer than one pixel
    // if so, top == bottom
    if (dir == 'y')
    {
        top = area.y2 - area.y1;
        left = ceil (area.x1) - area.x1;
        right = area.x2 - floor (area.x2);

        for (int x = (int) floor (area.x1); x < ceil (area.x2); x++)
        {
            double width = 1;
            if (x == floor (area.x1)) width = left;
            if (x == floor (area.x2)) width = right;

            addPixelToColor (color, inMatrix [x][(int) floor (area.y1)], width * top);
        }
    }

    // For stripes with breadth smaller than one but longer than one pixel
    // if so, left == right
    if (dir == 'x')
    {
        left = area.x2 - area.x1;
        top = ceil (area.y1) - area.y1;
        bottom = area.y2 - floor (area.y2);

        for (int y = (int) floor (area.y1); y < ceil (area.y2); y++)
        {
            double height = 1;
            if (y == floor (area.y1)) height = top;
            if (y == floor (area.y2)) height = bottom;

            addPixelToColor (color, inMatrix [(int) floor (area.x1)][y], height * left);
        }
    }
}

// Done
void addPixelToColor (Color * color, RGBTRIPLE pixel, double percentage)
{
    color->r += pixel.rgbtRed * percentage;
    color->g += pixel.rgbtGreen * percentage;
    color->b += pixel.rgbtBlue * percentage;
}

// Done
Color divColorByArea (Color colorSum, Area area)
{
    double percentage = calcArea (area);
    Color color =
    {
        .r = colorSum.r / percentage,
        .g = colorSum.g / percentage,
        .b = colorSum.b / percentage
    };
    return color;
}

// Done
RGBTRIPLE getPixelFromColor (Color color)
{
    RGBTRIPLE pixel =
    {
        .rgbtRed = color.r,
        .rgbtGreen = color.g,
        .rgbtBlue = color.b
    };
    return pixel;
}

// Done
double calcArea (Area area)
{
    return ((area.x2 - area.x1) * (area.y2 - area.y1));
}