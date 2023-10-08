#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int validate_key(string key);
string encode_text(string text, string cipher);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (!validate_key(argv[1]))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");
    string ciphertext = encode_text(plaintext, argv[1]);

    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

int validate_key(string key)
{
    // check for duplicates, invalid characters, and proper key length
    if (strlen(key) != 26)
    {
        return 0;
    }

    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!isalpha(key[i]))
        {
            return 0;
        }

        for (int j = 0; j < n; j++)
        {
            if (j != i && tolower(key[i]) == tolower(key[j]))
            {
                return 0;
            }
        }
    }

    return 1;
}

string encode_text(string text, string cipher)
{
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]) != 0)
        {
            // convert ASCII decimal of letter to index (from 0 to 25)
            int l_index = toupper(text[i]) - 65;

            // encode character
            if (isupper(text[i]))
            {
                text[i] = toupper(cipher[l_index]);
            }
            else
            {
                text[i] = tolower(cipher[l_index]);
            }
        }
    }

    return text;
}