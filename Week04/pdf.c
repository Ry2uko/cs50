#include <cs50.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./pdf FILENAME\n");
        return 1;
    }

    // Open file
    string filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error: No such file found.\n");
        return 1;
    }

    uint8_t buffer[4];
    fread(buffer, 1, 4, file);
    fclose(file);

    // Compare values
    uint8_t sequence[] = {37, 80, 68, 70};
    for (int i = 0; i < 4; i++)
    {
        if (sequence[i] != buffer[i])
        {
            printf("The file provided is NOT LIKELY a pdf file.\n");
            fclose(file);
            return 0;
        }
    }

    printf("The file provided is MOST LIKELY a pdf file.\n");
    fclose(file);
    return 0;
}