#include "helpers.h"
#include "math.h"
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE rgbtAverage =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = rgbtAverage;
            image[i][j].rgbtGreen = rgbtAverage;
            image[i][j].rgbtRed = rgbtAverage;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            WORD rgb_short[3]; // red green blue
            BYTE rgb_byte[3];
            rgb_short[0] =
                (WORD) round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen +
                             0.189 * image[i][j].rgbtBlue);
            rgb_short[1] =
                (WORD) round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen +
                             0.168 * image[i][j].rgbtBlue);
            rgb_short[2] =
                (WORD) round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen +
                             0.131 * image[i][j].rgbtBlue);

            for (int k = 0; k < 3; k++)
            {
                if (rgb_short[k] < 0)
                {
                    rgb_byte[k] = (BYTE) 0;
                }
                else if (rgb_short[k] > 255)
                {
                    rgb_byte[k] = (BYTE) 255;
                }
                else
                {
                    rgb_byte[k] = (BYTE) rgb_short[k];
                }
            }

            image[i][j].rgbtRed = rgb_byte[0];
            image[i][j].rgbtGreen = rgb_byte[1];
            image[i][j].rgbtBlue = rgb_byte[2];
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];

            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            image[i][width - j - 1].rgbtRed = tmp.rgbtRed;
            image[i][width - j - 1].rgbtGreen = tmp.rgbtGreen;
            image[i][width - j - 1].rgbtBlue = tmp.rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumR = 0, sumG = 0, sumB = 0, starting_row_index = i - 1, ending_row_index = i + 1,
                starting_col_index = j - 1, ending_col_index = j + 1;

            double cell_count = 0.0;
            if (i == 0)
            {
                starting_row_index = 0;
            }
            else if (i == height - 1)
            {
                ending_row_index = i;
            }

            if (j == 0)
            {
                starting_col_index = 0;
            }
            else if (j == width - 1)
            {
                ending_col_index = j;
            }

            for (int k = starting_row_index; k <= ending_row_index; k++)
            {
                for (int l = starting_col_index; l <= ending_col_index; l++)
                {
                    sumR += copy[k][l].rgbtRed;
                    sumG += copy[k][l].rgbtGreen;
                    sumB += copy[k][l].rgbtBlue;
                    cell_count++;
                }
            }
            image[i][j].rgbtRed = (BYTE) round(sumR / cell_count);
            image[i][j].rgbtGreen = (BYTE) round(sumG / cell_count);
            image[i][j].rgbtBlue = (BYTE) round(sumB / cell_count);
        }
    }
    return;
}
