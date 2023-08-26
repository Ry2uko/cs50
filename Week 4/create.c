#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check for improper usage, otherwise, get filename length
    if (argc != 2)
    {
        printf("Wrong usage: Try ./create [filename]\n");
        return 1;
    }
    int filename_length = strlen(argv[1]);

    // Create a new block of memory to store filename
    char *filename = malloc(sizeof(char) * (filename_length + 1));

    // Copy argv[1] into block of memory for filename
    sprintf(filename, "%s", argv[1]);

    // Open new file under the name stored at filename
    FILE *new_file = fopen(filename, "w");

    if (new_file == NULL)
    {
        printf("Error: Could not create file.\n");
        return 1;
    }

    free(filename);
    fclose(new_file);
}