#include <cs50.h>
#include <stdio.h>

bool prime(int number);

int main(void)
{
    int min;
    do
    {
        min = get_int("Minimum: ");
    }
    while (min < 1);

    int max;
    do
    {
        max = get_int("Maximum: ");
    }
    while (min >= max);

    for (int i = min; i <= max; i++)
    {
        if (prime(i))
        {
            printf("%i\n", i);
        }
    }
}

bool prime(int number)
{
    // TODO
    // if number is greater than 1 and (not divisible by 2 or equals to 2)
    if (number > 1 && (number % 2 != 0 || number == 2))
    {
        int i = 2;
        // check if number is divisible by i until it reaches its half
        while (i < (number / 2))
        {
            if (number % i == 0)
            {
                return false;
            }
            i++;
        }

        return true;
    }

    return false;
}
