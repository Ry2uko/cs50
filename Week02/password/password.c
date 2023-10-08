// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// TODO: Complete the Boolean function below
bool valid(string password)
{
    int has_lowercase = 0;
    int has_uppercase = 0;
    int has_number = 0;
    int has_symbol = 0;

    // ASCII Method
    for (int i = 0, n = strlen(password); i < n; i++)
    {
        if (password[i] >= 48 && password[i] <= 57)
        {
            // Numb3r (48~57)
            has_number = 1;
        }
        else if (password[i] >= 65 && password[i] <= 90)
        {
            // UPPERCASE (65~90)
            has_uppercase = 1;
        }
        else if (password[i] >= 97 && password[i] <= 122)
        {
            // lowercase (97~122)
            has_lowercase = 1;
        }
        else
        {
            // Symbol!
            has_symbol = 1;
        }
    }

    int valid_sum = has_uppercase + has_lowercase + has_symbol + has_number;
    if (valid_sum == 4)
    {
        return true;
    }

    return false;
}
