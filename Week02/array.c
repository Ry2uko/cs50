#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int size;
    do
    {
        size = get_int("Size: ");
    }
    while(size < 1);

    int powers[size];
    powers[0] = 1;

    for (int i = 1; i < size; i++)
    {
        powers[i] = i*2;
    }

    // Print array
    for (int i = 0; i < size; i++)
    {
        printf("%i\n", powers[i]);
    }
}