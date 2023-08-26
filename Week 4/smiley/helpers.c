#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // change to rgb(255, 128, 0) // orange
            RGBTRIPLE *img = &image[i][j];
            if (img->rgbtRed + img->rgbtGreen + img->rgbtBlue < 1)
            {
                img->rgbtRed = 0xff;
                img->rgbtGreen = 0x80;
                img->rgbtBlue = 0x00;
            }
        }
    }
}
