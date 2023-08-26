#include <cs50.h>
#include <stdio.h>

int factorial(int number);

int main(void)
{
    int n = get_int("Type a number: ");
    printf("%i\n", factorial(n));
}

int factorial(int number)
{
    if (number == 1)
    {
        return 1;
    }

    return number * factorial(number-1);
}