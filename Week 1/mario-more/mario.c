#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for a positive integer between 1 and 8 (inclusive)
    int height;
    do
    {
        height = get_int("Height (1-8): ");
    }
    while (height < 1 || height > 8);

    // Print pyramid
    for (int i = 1; i <= height; i++)
    {
        // left pyramid
        for (int j = height; j >= 1; j--)
        {
            j <= i ? printf("#") : printf(" ");
        }

        // gap
        printf("  ");

        // right pyramid
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}