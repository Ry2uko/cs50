#include "helpers.h"
#include <math.h>
#include <stdio.h>

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

            pixel->rgbtRed = rounded_avg;
            pixel->rgbtGreen = rounded_avg;
            pixel->rgbtBlue = rounded_avg;
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

            l += 1;
            r -= 1;
        }
    }

    return;
}

// helper function to get the new rgb values in the surround 3x3 grid at the provided coordinate
RGBTRIPLE blur_grid(int height, int width, int x, int y, RGBTRIPLE image[height][width])
{
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;
    int n = 0;

    for (int i = y - 1; i <= y + 1; i++)
    {
        for (int j = x - 1; j <= x + 1; j++)
        {
            // Handle boundary
            if (i >= 0 && i < height && j >= 0 && j < width)
            {
                sum_red += image[i][j].rgbtRed;
                sum_green += image[i][j].rgbtGreen;
                sum_blue += image[i][j].rgbtBlue;
                n++;
            }
        }
    }

    double avg_red = (float) sum_red / n;
    double avg_green = (float) sum_green / n;
    double avg_blue = (float) sum_blue / n;

    int rounded_avg_red = round(avg_red);
    int rounded_avg_green = round(avg_green);
    int rounded_avg_blue = round(avg_blue);

    /*
        BYTE  rgbtBlue;
        BYTE  rgbtGreen;
        BYTE  rgbtRed;
    */
    RGBTRIPLE pixel = {rounded_avg_blue, rounded_avg_green, rounded_avg_red};

    return pixel;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // get the filtered array
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

// helper function for calculating magnitude and capping it to 0 and 255
double calc_magnitude(int mx, int my)
{
    // sqrt(gx^2 + gy^2)
    double magnitude = sqrt(mx * mx + my * my);

    if (magnitude > 255)
    {
        magnitude = 255;
    }
    else if (magnitude < 0)
    {
        magnitude *= 0;
    }

    return magnitude;
}

// helper function for edges with similar algorithm to blur_grid
RGBTRIPLE edge_grid(int height, int width, int x, int y, RGBTRIPLE image[height][width])
{
    int mx_red = 0, my_red = 0;
    int mx_green = 0, my_green = 0;
    int mx_blue = 0, my_blue = 0;

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    int row = 0;
    for (int i = y - 1; i <= y + 1; i++)
    {
        int col = 0;
        for (int j = x - 1; j <= x + 1; j++)
        {
            // Handle boundary
            if (i >= 0 && i < height && j >= 0 && j < width)
            {
                // red
                mx_red += gx[row][col] * image[i][j].rgbtRed;
                my_red += gy[row][col] * image[i][j].rgbtRed;

                // green
                mx_green += gx[row][col] * image[i][j].rgbtGreen;
                my_green += gy[row][col] * image[i][j].rgbtGreen;

                // blue
                mx_blue += gx[row][col] * image[i][j].rgbtBlue;
                my_blue += gy[row][col] * image[i][j].rgbtBlue;
            }
            col++;
        }
        row++;
    }

    // calculate magnitude and cap to 0 and 255
    double mg_red = calc_magnitude(mx_red, my_red);
    double mg_green = calc_magnitude(mx_green, my_green);
    double mg_blue = calc_magnitude(mx_blue, my_blue);

    int rounded_mg_red = round(mg_red);
    int rounded_mg_green = round(mg_green);
    int rounded_mg_blue = round(mg_blue);

    /*
        BYTE  rgbtBlue;
        BYTE  rgbtGreen;
        BYTE  rgbtRed;
    */
    RGBTRIPLE pixel = {rounded_mg_blue, rounded_mg_green, rounded_mg_red};

    return pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // get the filtered array
    RGBTRIPLE edged_pixels[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            edged_pixels[i][j] = edge_grid(height, width, j, i, image);
        }
    }

    // modify image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edged_pixels[i][j];
        }
    }

    return;
}
