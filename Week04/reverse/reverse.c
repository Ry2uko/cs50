#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("File not found.\n");
        return 1;
    }

    // Read header
    WAVHEADER wh;
    fread(&wh, sizeof(WAVHEADER), 1, inptr);

    // Use check_format to ensure WAV format
    if (!check_format(wh))
    {
        fclose(inptr);
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *outptr = fopen(argv[2], "w");
    if (outptr == NULL)
    {
        printf("Error: Unable to open output file.\n");
        return 1;
    }

    // Write header to file
    fwrite(&wh, sizeof(WAVHEADER), 1, outptr);

    // Use get_block_size to calculate size of block
    const int BLOCK_SIZE = get_block_size(wh);

    // Write reversed audio to file
    // https://cs50.stackexchange.com/questions/43626/wav-file-incorrectly-reversing
    fseek(inptr, BLOCK_SIZE, SEEK_END);
    while (ftell(inptr) - BLOCK_SIZE > sizeof(wh))
    {
        fseek(inptr, -2 * BLOCK_SIZE, SEEK_CUR);
        BYTE block[BLOCK_SIZE];
        fread(&block, BLOCK_SIZE, 1, inptr);
        fwrite(&block, BLOCK_SIZE, 1, outptr);
    }

    fclose(inptr);
    fclose(outptr);
    return 0;
}

int check_format(WAVHEADER header)
{
    // format should spell out 'WAVE'
    char *format = malloc(sizeof(char) * 5);
    if (format == NULL)
    {
        printf("Error: Something went wrong.");
        return 0;
    }

    for (int i = 0; i < 4; i++)
    {
        *(format + i) = header.format[i];
        if (i == 3)
        {
            *(format + i + 1) = '\0';
        }
    }

    if (strcmp(format, "WAVE") == 0)
    {
        free(format);
        return 1;
    }

    free(format);
    return 0;
}

int get_block_size(WAVHEADER header)
{
    int channels = header.numChannels;
    int bytes_per_sample = header.bitsPerSample / 8;

    return channels * bytes_per_sample;
}