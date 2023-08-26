// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string replace(string str_input);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./no-vowels word\n");
        return 1;
    }

    printf("%s\n", replace(argv[1]));
    return 0;
}

string replace(string str_input)
{
    int n = strlen(str_input);

    for (int i = 0; i < n; i++)
    {
        char c_lower = tolower(str_input[i]);

        switch (c_lower)
        {
            case 'a':
                str_input[i] = '6';
                break;
            case 'i':
                str_input[i] = '1';
                break;
            case 'e':
                str_input[i] = '3';
                break;
            case 'o':
                str_input[i] = '0';
        }
    }

    return str_input;
}
