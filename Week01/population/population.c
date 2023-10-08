#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int start_size;
    do
    {
        start_size = get_int("Starting population size: ");
    }
    while (start_size < 9);

    // TODO: Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("Ending population size: ");
    }
    while (end_size < start_size);

    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    while (start_size < end_size)
    {
        int n1 = start_size / 3;
        int n2 = start_size / 4;
        start_size += n1;
        start_size -= n2;
        years++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", years);
}
