// MARIO LESS

#include <stdio.h>
#include <cs50.h>

int main()
{
    // Variable initialisation
    int height;

    // Input validation
    do
    {
        height = get_int("Height of pyramid (0-23): ");
    }
    while (height < 0 || height > 23);

    // Printing part
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height + 1; j++)
        {
            if (j < height - i - 1) printf(" ");
            else printf("#");
        }
        printf("\n");
    }
    return 0;
}