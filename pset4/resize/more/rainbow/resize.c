#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../bmp.h"

#define MAX_FACTOR 1000.0

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
// Actual algorithm; takes the matrix with pixels of the input and of the output BMP, the size of the latter and the resize factor
void resize (RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int inWidth, int inHeight, int outWidth, int outHeight, double factor);

// Function to scale a pixel from the output matrix to an area in the input matrix
void rescalePixel (RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int x, int y, int inWidth, int inHeight, int outWidth, int outHeight, double factor);

// Function to calculate the average color in a given area, by dividing the color sum by the area
void averageColor (Color * color, RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int x, int y, int inWidth, int inHeight, int outWidth, int outHeight, double factor);

// Helper function to calculate the corners of the area
double getCorner (int a, double factor);

// Calculate the area of an Area...
double calcArea (Area * area);

// Iterates over each pixel in a given area and sums up the colors multiplied by the percentage they represent in the area
void sumUpColors (Color * color, RGBTRIPLE * inMatrix [], Area * area);

// Takes the values from a color and writes it into the pixel
void getPixelFromColor (RGBTRIPLE * pixel, Color * color);

// Function to calculate the amount of full pixels per height or width
int calcFullPixAmount (double start, double end);

// Takes the values from a pixel and adds it to a Color struct
void addColor (Color * color, RGBTRIPLE * pixel, double percentage);

// Function to print the values of an area
void pprint (Area * area);
/*=====================================*/

int main(int argc, char *argv[])
{
    // Input Validation
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [0 < new Size < %f]\n", argv[0], MAX_FACTOR);
        return 1;
    }

    // n = resize factor
    int newSize = atoi(argv[1]);
    if (newSize <= 0 || newSize > MAX_FACTOR)
    {
        fprintf(stderr, "New Size n must be positive and max %f\n", MAX_FACTOR);
        return 5;
    }

    /*#######################################################*/
    // Variable initialization (input and output file)

    char tmp [20];
    strcat (tmp, argv[1]);
    strcat (tmp, "x");
    strcat (tmp, argv[1]);
    strcat (tmp, ".bmp");

    char output [14];
    int j = 0;
    for (int i = 0; i < 14; i++)
    {
        if (tmp[i] != '\0')
        {
            output[j] = tmp[i];
            j++;
        }
    }
    output[j] = '\0';
    /*#######################################################*/

    // Open files
    FILE * in = fopen("rainbow.bmp", "r");
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

    // Read BITMAP HEADER of input file
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, in);
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, in);

    // Recalculate resize factor
    double factor = (double) (newSize / bi.biWidth);

    // Make values positive (they don't have to)
    bi.biWidth = abs(bi.biWidth);
    bi.biHeight = abs(bi.biHeight);

    // Values of input file
    int inWidth = bi.biWidth;
    int inHeight = bi.biHeight;

    // Values of output file
    int outWidth = (int) factor * inWidth;
    int outHeight = (int) factor * inHeight;

    // Adapting bitmap headers
    bf.bfSize = (int) (pow(factor, 2) * (bf.bfSize - 54) + 54);
    bi.biSizeImage = (int) pow(factor, 2) * bi.biSizeImage;
    bi.biWidth = outWidth;
    bi.biHeight = outHeight;



    // Write BITMAP HEADER of output file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out);


    // Calculate padding
    int paddin = (4 - (inWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddout = (4 - (outWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Create a scanfile the size of the input BMP (manually allocating mem)
    RGBTRIPLE * inputMatrix [inWidth];
    for (int i = 0; i < inWidth; i++) { inputMatrix [i] = malloc (inHeight * sizeof (RGBTRIPLE)); }

    for (int x = 0; x < inWidth; x++)
    {
        for (int y = 0; y < inHeight; y++)
        {
            // Scan each pixel of the input BMP and write it into the map
            RGBTRIPLE scan;
            fread(&scan, sizeof(RGBTRIPLE), 1, in);
            inputMatrix[x][y] = scan;
        }
        // Skip Padding
        fseek(in, paddin, SEEK_CUR);
    }

    RGBTRIPLE * outputMatrix [outWidth];
    for (int i = 0; i < outWidth; i++) { outputMatrix [i] = malloc (outHeight * sizeof (RGBTRIPLE)); }

    printf("Resize from [%i|%i] to [%i|%i].\n", inWidth, inHeight, outWidth, outHeight);

    // RESIZING ALGORITHM
    resize (inputMatrix, outputMatrix, inWidth, inHeight, outWidth, outHeight, factor);

    // iterate over infile's scanlines (over the size of the map)
    for (int y = 0; y < outHeight; y++)
    {
        // iterate over pixels in scanline
        for (int x = 0; x < outWidth; x++)
        {
            fwrite(& outputMatrix[x][y], sizeof(RGBTRIPLE), 1, out);
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

void resize (RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int inWidth, int inHeight, int outWidth, int outHeight, double factor)
{
    // Iterate over every pixel of the output Matrix
    for (int y = 0; y < outHeight; y++)
    {
        for (int x = 0; x < outWidth; x++)
        {
            rescalePixel (inMatrix, outMatrix, x, y, inWidth, inHeight, outWidth, outHeight, factor);
        }
    }
}

void rescalePixel (RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int x, int y, int inWidth, int inHeight, int outWidth, int outHeight, double factor)
{
    // Create a struct to store color values in (used for calculating the average)
    // and average the color in a calculated area in the input matrix
    Color color =
    {
        .r = 0,
        .g = 0,
        .b = 0

    };
    averageColor (& color, inMatrix, outMatrix, x, y, inWidth, inHeight, outWidth, outHeight, factor);

    // Converts the color into a pixel and stores it at the position (x|y) in the output matrix
    RGBTRIPLE * pixel = malloc (sizeof (RGBTRIPLE));
    getPixelFromColor (pixel, & color);
    outMatrix [x][y] = * pixel;

}

void averageColor (Color * color, RGBTRIPLE * inMatrix [], RGBTRIPLE * outMatrix [], int x, int y, int inWidth, int inHeight, int outWidth, int outHeight, double factor)
{
    // Create a struct to store the area in
    Area area =
    {
        .x1 = getCorner (x, factor),
        .y1 = getCorner (y, factor),
        .x2 = getCorner (x + 1, factor),
        .y2 = getCorner (y + 1, factor)
    };

    // Calculate the area that the pixel in the output matrix takes up in the input matrix
    double areaSize = calcArea (& area);

    // Take all colors present in this area and sum them up
    sumUpColors (color, inMatrix, & area);

    color->r = (int) round (color->r / areaSize);
    color->g = (int) round (color->g / areaSize);
    color->b = (int) round (color->b / areaSize);
}

double getCorner (int a, double factor)
{
    return (double) a / factor;
}

double calcArea (Area * area)
{
    return ((area->x2 - area->x1) * (area->y2 - area->y1));
}

void sumUpColors (Color * color, RGBTRIPLE * inMatrix [], Area * area)
{
    int pixXamount = calcFullPixAmount (area->x1, area->x2);
    int pixYamount = calcFullPixAmount (area->y1, area->y2);

    // Calculates the size of the margin
    double startX = ceil (area->x1) - area->x1;
    double startY = ceil (area->y1) - area->y1;
    double endX = floor (area->x2) - area->x2;
    double endY = floor (area->y2) - area->y2;

    if (ceil (area->x2) - floor (area->x1) == 1.0 && ceil (area->y2) - floor (area->y1) == 1.0)
    {
        addColor (color, & inMatrix [(int) floor (area->x1)][(int) floor (area->y1)], calcArea (area));
        return;
    }

    // Iterate over each pixel in the area, checking wether it's whole or not
    for (int y = 0; y < pixYamount + 2; y++)
    {
        // Check for margin:
        if ((y == 0 && startY > 0) || (y == pixYamount + 1 && endY > 0) || (y > 0 && y < pixYamount + 1))
        {
            // Helper var for y margin (to calculate the percentage)
            double tdy = 1;

            // if there's a y margin, use that as temp field height (for percentage calculation)
            if (y == 0 && startY > 0) tdy = startY;
            else if (y == pixYamount + 1 && endY > 0) tdy = endY;

            for (int x = 0; x < pixXamount + 2; x++)
            {
                // is there a margin? otherwise jump over first / last x
                if ((x == 0 && startX > 0) || (x == pixXamount + 1 && endX > 0) || (x > 0 && x < pixXamount + 1))
                {
                    // Helper var for x margin
                    double tdx = 1;

                    // if there's an x margin, use that as temp field width (for percentage calculation)
                    if (x == 0 && startX > 0) tdx = startX;
                    else if (x == pixXamount + 1 && endX > 0) tdx = endX;

                    // Add the color of this pixel multiplied by it's percentage to the overall sum
                    addColor (color, & inMatrix [(int) floor (area->x1) + x][(int) floor (area->y1) + y], tdx * tdy);
                }
            }
        }
    }
}

void getPixelFromColor (RGBTRIPLE * pixel, Color * color)
{
    pixel->rgbtRed = color->r;
    pixel->rgbtGreen = color->g;
    pixel->rgbtBlue = color->b;
}

int calcFullPixAmount (double start, double end)
{
    return (int) floor (end) - ceil (start);
}

void addColor (Color * color, RGBTRIPLE * pixel, double percentage)
{
    color->r += pixel->rgbtRed * percentage;
    color->g += pixel->rgbtGreen * percentage;
    color->b += pixel->rgbtBlue * percentage;
}