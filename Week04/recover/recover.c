#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    const int BLOCK_SIZE = 512;

    BYTE buffer[BLOCK_SIZE];
    FILE *img = NULL;
    int img_count = 0;

    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // if jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (img != NULL)
            {
                fclose(img);
            }

            // "###.jpg\0"
            char img_filename[8];
            sprintf(img_filename, "%03i.jpg", img_count);

            img = fopen(img_filename, "w");
            img_count++;
        }

        if (img != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, img);
        }
    }

    fclose(card);
    fclose(img);
    return 0;
}