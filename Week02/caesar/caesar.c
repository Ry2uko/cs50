#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    int key = atoi(argv[1]) % 26;
    string plaintext = get_string("plaintext: ");
    char ciphertext[strlen(plaintext)];

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // ASCII: A-Z:65-90, a-z:97-122, length: 26
        int c = plaintext[i];
        if (isalpha(c))
        {
            c += key;
            while ((isupper(plaintext[i]) && c > 90) || (islower(plaintext[i]) && c > 122))
            {
                c -= 26;
            }
        }

        ciphertext[i] = c;
    }
    ciphertext[strlen(plaintext)] = '\0';

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}