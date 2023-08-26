#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *get_grid(int height, int width, int x, int y, int grid_main[height][width]);

int main(void)
{
    // Practice 9x9 grid helper function in filter-more pset
    int height = 6;
    int width = 6;
    int grid_main[height][width];

    // Seed the random number generator
    srand(time(NULL));

    // Randomize grid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get a random value between 0-10
            int r = rand() % 11;
            grid_main[i][j] = r;
            printf("%i ", r);
        }
        printf("\n");
    }


    printf("\n");

    // get surrounding 3x3 at provided grid's coordinate
    int *grid = get_grid(height, width, 0, 0, grid_main);


    // print grid at provided grid's coordinate at get_grid
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%i ", grid[i*3 + j]);
        }
        printf("\n");
    }

    free(grid);
    return 0;
}

int *get_grid(int height, int width, int x, int y, int grid_main[height][width])
{
    int *grid = malloc(9 * sizeof(int));

    int row = 0;
    for (int i = y-1; i <= y+1; i++)
    {
        int col = 0;
        for (int j = x-1; j <= x+1; j++)
        {
            // Handle boundary
            if (i >= 0 && i < height && j >= 0 && j < width)
            {
                grid[row * 3 + col] = grid_main[i][j];
            }
            else
            {
                grid[row * 3 + col] = -1;
            }
            col++;
        }
        row++;
    }

    return grid;
}