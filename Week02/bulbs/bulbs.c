#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    string message = get_string("Message: ");

    // convert to binary
    int length = strlen(message);

    for (int i = 0; i < length; i++)
    {
        int n = 128;
        int mn = message[i];

        char binary[] = "00000000";

        // Add ones
        for (int k = 0; k < 8; k++)
        {
            if (mn >= n)
            {
                binary[k] = '1';
                mn %= n;
            }

            n /= 2;

            if (n == 0)
            {
                break;
            }
        }

        // print binary
        for (int q = 0; q < 8; q++)
        {
            int bit = binary[q] - '0';
            print_bulb(bit);
        }
        printf("\n");
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
