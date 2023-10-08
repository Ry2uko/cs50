#include <stdio.h>
#include <cs50.h>

int main(void)
{
    const int size = get_int("Size: ");
    int grid[size];

    // Create grid
    for (int i = 0; i < size; i++)
    {
        int row[size];
        for (int j = 0; j < size; j++)
        {
            grid[i][j] = 0;
        }
        grid[i] = row;
    }


    // Print grid
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%i ", grid[i][j]);
        }
        printf("\n");
    }

    return 0;
}