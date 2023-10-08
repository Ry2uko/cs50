#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(string input);

int main(void)
{
    string input = get_string("Enter a positive integer: ");

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }

    // Convert string to int
    printf("%i\n", convert(input));
}

int convert(string input)
{
    int l_index = strlen(input) - 1;
    char last = input[l_index] - '0';
    input[l_index] = '\0';

    if (strlen(input) <= 0)
    {
        return last;
    }

    return last + convert(input) * 10;
}