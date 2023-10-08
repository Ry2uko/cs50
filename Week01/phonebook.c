#include <cs50.h>
#include <stdio.h>

int main(void)
{
    string name = get_string("What's your name? ");
    int age = get_int("How old are you? ");
    string phoneNumber = get_string("What is your phone number? ");

    printf("\nHi, %s!\n", name);
    printf("You are %i years old.\n", age);
    printf("And your phone number is %s.\n", phoneNumber);
}