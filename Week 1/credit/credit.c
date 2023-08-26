#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool is_valid(string card);
string get_format(string card);

int main(void)
{
    // Get input
    long card;
    do
    {
        // get only positive input
        card = get_long("Number: ");
    }
    while (card < 0);

    // Convert card to string
    char card_str[256];
    sprintf(card_str, "%li", card);

    // Validate card
    string card_format = get_format(card_str);
    bool valid = false;
    if (strcmp(card_format, "INVALID") != 0)
    {
        valid = is_valid(card_str);
        if (valid)
        {
            printf("%s\n", card_format);
        }
    }

    if (!valid)
    {
        printf("INVALID\n");
    }
}

// Get credit card number format
string get_format(string card)
{
    if (card[0] == '3' && strlen(card) == 15)
    {
        // American Express (34, 37) and 15-digit
        if (card[1] == '4' || card[1] == '7')
        {
            return "AMEX";
        }
    }
    else if (card[0] == '4' && (strlen(card) == 13 || strlen(card) == 16))
    {
        // Visa (4x) and 13- or 16-digit
        return "VISA";
    }
    else if (card[0] == '5' && strlen(card) == 16)
    {
        // Mastercard (51, 52, 53, 54, 55) and 16-digit
        const char *m[5] = {"1", "2", "3", "4", "5"};
        int numElements = sizeof(m) / sizeof(m[0]);
        bool found = false;

        // check if second digit is in the range of 1-5
        for (int i = 0; i < numElements; i++)
        {
            if (card[1] == m[i][0])
            {
                found = true;
            }
        }

        if (found)
        {
            return "MASTERCARD";
        }
    }

    return "INVALID";
}

// Checksum (Luhn’s Algorithm)
bool is_valid(string card)
{
    int sum = 0;
    for (int i = 1; i <= strlen(card); i++)
    {
        int j = strlen(card) - i;  // index at reversed order
        int digit = card[j] - '0'; // convert to int

        // starts at index 14 or second-to-last
        if (i % 2 == 0)
        {
            int digit_twice = digit * 2; // range: 0-18

            // add each digit to sum including multiple digits
            if (digit_twice >= 10)
            {
                // 18: 1 + 8 = 9 or 18 - 9 = 9
                // 10: 1 + 0 = 1 or 10 - 9 = 1
                sum += digit_twice - 9;
            }
            else
            {
                sum += digit_twice;
            }
        }
        else
        {
            sum += digit;
        }
    }

    return sum % 10 == 0 ? true : false;
}
