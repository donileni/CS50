#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            int average = round((red + green + blue) / 3.0);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            int sepiRed = round(0.393 * red + 0.769 * green + 0.189 * blue);
            if (sepiRed > 255)
            {
                sepiRed = 255;
            }

            int sepiGreen = round(0.349 * red + 0.686 * green + 0.168 * blue);
            if (sepiGreen > 255)
            {
                sepiGreen = 255;
            }

            int sepiBlue = round(0.272 * red + 0.534 * green + 0.131 * blue);
            if (sepiBlue > 255)
            {
                sepiBlue = 255;
            }

            image[i][j].rgbtRed = sepiRed;
            image[i][j].rgbtGreen = sepiGreen;
            image[i][j].rgbtBlue = sepiBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            temp[i][j].rgbtRed = image[i][j].rgbtRed;
            temp[i][j].rgbtGreen = image[i][j].rgbtGreen;
            temp[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][(width - 1) - j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][(width - 1) - j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][(width - 1) - j].rgbtBlue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    double averageRed;
    double averageGreen;
    double averageBlue;
    float counter;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            averageRed = 0;
            averageGreen = 0;
            averageBlue = 0;
            counter = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    int m = i + k;
                    int n = j + l;

                    if (m < height && n < width && m >= 0 && n >= 0)
                    {
                        averageRed += image[m][n].rgbtRed;
                        averageGreen += image[m][n].rgbtGreen;
                        averageBlue += image[m][n].rgbtBlue;
                        counter++;
                    }
                }
            }

            averageRed = round(averageRed / counter);
            averageGreen = round(averageGreen / counter);
            averageBlue = round(averageBlue / counter);

            temp[i][j].rgbtRed = averageRed;
            temp[i][j].rgbtGreen = averageGreen;
            temp[i][j].rgbtBlue = averageBlue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }



    return;
}
