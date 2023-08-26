#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];

            int red = pixel->rgbtRed;
            int green = pixel->rgbtGreen;
            int blue = pixel->rgbtBlue;

            double avg = (red + green + blue) / 3.0;
            int rounded_avg = round(avg);

            RGBTRIPLE new_pixel = {rounded_avg, rounded_avg, rounded_avg};
            *pixel = new_pixel;
        }
    }

    return;
}

// helper function for calculating sepia and capping at 0 and 255
int calculate_sepia(RGBTRIPLE pixel, char sepia_color)
{
    int r = pixel.rgbtRed;
    int g = pixel.rgbtGreen;
    int b = pixel.rgbtBlue;

    double sepia;
    switch (sepia_color)
    {
        case 'r':
            sepia = 0.393 * r + 0.769 * g + 0.189 * b;
            break;
        case 'g':
            sepia = 0.349 * r + 0.686 * g + 0.168 * b;
            break;
        case 'b':
            sepia = 0.272 * r + 0.534 * g + 0.131 * b;
    }

    if (sepia > 255)
    {
        sepia = 255;
    }
    else if (sepia < 0)
    {
        sepia = 0;
    }

    int rounded_sepia = round(sepia);
    return rounded_sepia;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];

            int sepia_red = calculate_sepia(*pixel, 'r');
            int sepia_green = calculate_sepia(*pixel, 'g');
            int sepia_blue = calculate_sepia(*pixel, 'b');

            RGBTRIPLE new_pixel = {sepia_blue, sepia_green, sepia_red};
            *pixel = new_pixel;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int l = 0, r = width - 1;

        while (l < r)
        {
            RGBTRIPLE temp = image[i][l];
            image[i][l] = image[i][r];
            image[i][r] = temp;

            l++;
            r--;
        }
    }

    return;
}

// helper function to get the new rgb values in the surround 3x3 grid at the provided coordinate
RGBTRIPLE blur_grid(int height, int width, int x, int y, RGBTRIPLE image[height][width])
{
    // sum and count of cells
    int sr = 0, sg = 0, sb = 0;
    int n = 0;

    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            // Handle boundary
            if (i >= 0 && i < height && j >= 0 && j < width)
            {
                RGBTRIPLE *pixel = &image[i][j];
                sr += pixel->rgbtRed;
                sg += pixel->rgbtGreen;
                sb += pixel->rgbtBlue;
                n++;
            }
        }
    }

    // calculate reciprocal of n
    double rn = 1.0 / n;

    int avgr = round(sr * rn);
    int avgg = round(sg * rn);
    int avgb = round(sb * rn);

    RGBTRIPLE new_pixel = {avgb, avgg, avgr};
    return new_pixel;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // get the blurred pixels
    RGBTRIPLE blurred_pixels[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blurred_pixels[i][j] = blur_grid(height, width, j, i, image);
        }
    }

    // modify image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred_pixels[i][j];
        }
    }

    return;
}
