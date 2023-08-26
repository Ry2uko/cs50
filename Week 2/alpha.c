#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main(void)
{
    string word = get_string("Word: ");

    for (int i = 0, n = strlen(word), curr_decimal = 0; i < n; i++)
    {
        if (word[i] < curr_decimal)
        {
            printf("No\n");
            return 0;
        }

        curr_decimal = word[i];
    }

    printf("Yes\n");
    return 0;
}