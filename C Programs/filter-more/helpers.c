#include "helpers.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        BYTE newcolor = 0;
        for (int j = 0; j < width; j++)
        {
            newcolor = round((double)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);

            image[i][j].rgbtBlue = newcolor;
            image[i][j].rgbtGreen = newcolor;
            image[i][j].rgbtRed = newcolor;
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
            RGBTRIPLE temp;
            temp.rgbtBlue = image[i][j].rgbtBlue;
            temp.rgbtRed = image[i][j].rgbtRed;
            temp.rgbtGreen = image[i][j].rgbtGreen;

            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;

            image[i][width - 1 - j].rgbtBlue = temp.rgbtBlue;
            image[i][width - 1 - j].rgbtRed = temp.rgbtRed;
            image[i][width - 1 - j].rgbtGreen = temp.rgbtGreen;
       }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE **imagecopy = (RGBTRIPLE **)malloc(height * sizeof(RGBTRIPLE *));

    for (int i = 0; i < height; i++)
    {
        imagecopy[i] = (RGBTRIPLE *)calloc(width, sizeof(RGBTRIPLE));
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            imagecopy[i][j].rgbtRed = image[i][j].rgbtRed;
            imagecopy[i][j].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0)
            {
                if (j == 0)
                {
                    image[i][j].rgbtBlue = round((double)(imagecopy[i][j].rgbtBlue + imagecopy[i][j + 1].rgbtBlue +
                                           imagecopy[i + 1][j].rgbtBlue + imagecopy[i + 1][j + 1].rgbtBlue) / 4);
                    image[i][j].rgbtRed = round((double)(imagecopy[i][j].rgbtRed + imagecopy[i][j + 1].rgbtRed +
                                          imagecopy[i + 1][j].rgbtRed + imagecopy[i + 1][j + 1].rgbtRed) / 4);
                    image[i][j].rgbtGreen = round((double)(imagecopy[i][j].rgbtGreen + imagecopy[i][j + 1].rgbtGreen +
                                            imagecopy[i + 1][j].rgbtGreen + imagecopy[i + 1][j + 1].rgbtGreen) / 4);
                }
                else if (j == width - 1)
                {
                    image[i][j].rgbtBlue = round((double)(imagecopy[i][j].rgbtBlue + imagecopy[i][j - 1].rgbtBlue +
                                           imagecopy[i + 1][j].rgbtBlue + imagecopy[i + 1][j - 1].rgbtBlue) / 4);
                    image[i][j].rgbtRed = round((double)(imagecopy[i][j].rgbtRed + imagecopy[i][j - 1].rgbtRed +
                                          imagecopy[i + 1][j].rgbtRed + imagecopy[i + 1][j - 1].rgbtRed) / 4);
                    image[i][j].rgbtGreen = round((double)(imagecopy[i][j].rgbtGreen + imagecopy[i][j - 1].rgbtGreen +
                                            imagecopy[i + 1][j].rgbtGreen + imagecopy[i + 1][j - 1].rgbtGreen) / 4);
                }
                else
                {
                    image[i][j].rgbtBlue = round((double)(imagecopy[i][j].rgbtBlue + imagecopy[i][j + 1].rgbtBlue + imagecopy[i][j - 1].rgbtBlue +
                                           imagecopy[i + 1][j - 1].rgbtBlue + imagecopy[i + 1][j].rgbtBlue + imagecopy[i + 1][j + 1].rgbtBlue) / 6);
                    image[i][j].rgbtRed = round((double)(imagecopy[i][j].rgbtRed + imagecopy[i][j + 1].rgbtRed + imagecopy[i][j - 1].rgbtRed +
                                          imagecopy[i + 1][j - 1].rgbtRed + imagecopy[i + 1][j].rgbtRed + imagecopy[i + 1][j + 1].rgbtRed) / 6);
                    image[i][j].rgbtGreen = round((double)(imagecopy[i][j].rgbtGreen + imagecopy[i][j + 1].rgbtGreen + imagecopy[i][j - 1].rgbtGreen +
                                            imagecopy[i + 1][j - 1].rgbtGreen + imagecopy[i + 1][j].rgbtGreen + imagecopy[i + 1][j + 1].rgbtGreen) / 6);
                }
            }

            else if (i == height - 1)
            {
                if (j == 0)
                {
                    image[i][j].rgbtBlue = round((double)(imagecopy[i][j].rgbtBlue + imagecopy[i - 1][j].rgbtBlue +
                                           imagecopy[i - 1][j + 1].rgbtBlue + imagecopy[i][j + 1].rgbtBlue) / 4);
                    image[i][j].rgbtRed = round((double)(imagecopy[i][j].rgbtRed + imagecopy[i - 1][j].rgbtRed +
                                          imagecopy[i - 1][j + 1].rgbtRed + imagecopy[i][j + 1].rgbtRed) / 4);
                    image[i][j].rgbtGreen = round((double)(imagecopy[i][j].rgbtGreen + imagecopy[i - 1][j].rgbtGreen +
                                            imagecopy[i - 1][j + 1].rgbtGreen + imagecopy[i][j + 1].rgbtGreen) / 4);
                }
                else if (j == width - 1)
                {
                    image[i][j].rgbtBlue = round((double)(imagecopy[i][j].rgbtBlue + imagecopy[i - 1][j].rgbtBlue +
                                           imagecopy[i - 1][j - 1].rgbtBlue + imagecopy[i][j - 1].rgbtBlue) / 4);
                    image[i][j].rgbtRed = round((double)(imagecopy[i][j].rgbtRed + imagecopy[i - 1][j].rgbtRed +
                                          imagecopy[i - 1][j - 1].rgbtRed + imagecopy[i][j - 1].rgbtRed) / 4);
                    image[i][j].rgbtGreen = round((double)(imagecopy[i][j].rgbtGreen + imagecopy[i - 1][j].rgbtGreen +
                                            imagecopy[i - 1][j - 1].rgbtGreen + imagecopy[i][j - 1].rgbtGreen) / 4);
                }
                else
                {
                    image[i][j].rgbtBlue = round((double)(imagecopy[i][j - 1].rgbtBlue + imagecopy[i][j].rgbtBlue + imagecopy[i][j + 1].rgbtBlue +
                                           imagecopy[i - 1][j - 1].rgbtBlue + imagecopy[i - 1][j].rgbtBlue + imagecopy[i - 1][j + 1].rgbtBlue) / 6);
                    image[i][j].rgbtRed = round((double)(imagecopy[i][j - 1].rgbtRed + imagecopy[i][j].rgbtRed + imagecopy[i][j + 1].rgbtRed +
                                          imagecopy[i - 1][j - 1].rgbtRed + imagecopy[i - 1][j].rgbtRed + imagecopy[i - 1][j + 1].rgbtRed) / 6);
                    image[i][j].rgbtGreen = round((double)(imagecopy[i][j - 1].rgbtGreen + imagecopy[i][j].rgbtGreen + imagecopy[i][j + 1].rgbtGreen +
                                            imagecopy[i - 1][j - 1].rgbtGreen + imagecopy[i - 1][j].rgbtGreen + imagecopy[i - 1][j + 1].rgbtGreen) / 6);
                }
            }

            else if (j == 0)
            {
                image[i][j].rgbtBlue = round((double)(imagecopy[i - 1][j].rgbtBlue + imagecopy[i][j].rgbtBlue + imagecopy[i + 1][j].rgbtBlue +
                                       imagecopy[i - 1][j + 1].rgbtBlue + imagecopy[i][j + 1].rgbtBlue + imagecopy[i + 1][j + 1].rgbtBlue) / 6);
                image[i][j].rgbtRed = round((double)(imagecopy[i - 1][j].rgbtRed + imagecopy[i][j].rgbtRed + imagecopy[i + 1][j].rgbtRed +
                                      imagecopy[i - 1][j + 1].rgbtRed + imagecopy[i][j + 1].rgbtRed + imagecopy[i + 1][j + 1].rgbtRed) / 6);
                image[i][j].rgbtGreen = round((double)(imagecopy[i - 1][j].rgbtGreen + imagecopy[i][j].rgbtGreen + imagecopy[i + 1][j].rgbtGreen +
                                        imagecopy[i - 1][j + 1].rgbtGreen + imagecopy[i][j + 1].rgbtGreen + imagecopy[i + 1][j + 1].rgbtGreen) / 6);
            }

            else if (j == width - 1)
            {
                image[i][j].rgbtBlue = round((double)(imagecopy[i - 1][j].rgbtBlue + imagecopy[i][j].rgbtBlue + imagecopy[i + 1][j].rgbtBlue +
                                       imagecopy[i - 1][j - 1].rgbtBlue + imagecopy[i][j - 1].rgbtBlue + imagecopy[i + 1][j - 1].rgbtBlue) / 6);
                image[i][j].rgbtRed = round((double)(imagecopy[i - 1][j].rgbtRed + imagecopy[i][j].rgbtRed + imagecopy[i + 1][j].rgbtRed +
                                      imagecopy[i - 1][j - 1].rgbtRed + imagecopy[i][j - 1].rgbtRed + imagecopy[i + 1][j - 1].rgbtRed) / 6);
                image[i][j].rgbtGreen = round((double)(imagecopy[i - 1][j].rgbtGreen + imagecopy[i][j].rgbtGreen + imagecopy[i + 1][j].rgbtGreen +
                                        imagecopy[i - 1][j - 1].rgbtGreen + imagecopy[i][j - 1].rgbtGreen + imagecopy[i + 1][j - 1].rgbtGreen) / 6);
            }
            else
            {
                image[i][j].rgbtBlue = round((double)(imagecopy[i - 1][j - 1].rgbtBlue + imagecopy[i - 1][j].rgbtBlue + imagecopy[i - 1][j + 1].rgbtBlue +
                                       imagecopy[i][j - 1].rgbtBlue + imagecopy[i][j].rgbtBlue + imagecopy[i][j + 1].rgbtBlue +
                                       imagecopy[i + 1][j - 1].rgbtBlue + imagecopy[i + 1][j].rgbtBlue + imagecopy[i + 1][j + 1].rgbtBlue) / 9);

                image[i][j].rgbtRed = round((double)(imagecopy[i - 1][j - 1].rgbtRed + imagecopy[i - 1][j].rgbtRed + imagecopy[i - 1][j + 1].rgbtRed +
                                      imagecopy[i][j - 1].rgbtRed + imagecopy[i][j].rgbtRed + imagecopy[i][j + 1].rgbtRed +
                                      imagecopy[i + 1][j - 1].rgbtRed + imagecopy[i + 1][j].rgbtRed + imagecopy[i + 1][j + 1].rgbtRed) / 9);

                image[i][j].rgbtGreen = round((double)(imagecopy[i - 1][j - 1].rgbtGreen + imagecopy[i - 1][j].rgbtGreen + imagecopy[i - 1][j + 1].rgbtGreen +
                                        imagecopy[i][j - 1].rgbtGreen + imagecopy[i][j].rgbtGreen + imagecopy[i][j + 1].rgbtGreen +
                                        imagecopy[i + 1][j - 1].rgbtGreen + imagecopy[i + 1][j].rgbtGreen + imagecopy[i + 1][j + 1].rgbtGreen) / 9);
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        if (imagecopy[i])
        {
            free(imagecopy[i]);
        }
    }

    if (imagecopy)
    {
        free(imagecopy);
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE **imagecopy = (RGBTRIPLE **)malloc((height + 2) * sizeof(RGBTRIPLE *));

    for (int i = 0; i < height + 2; i++)
    {
        imagecopy[i] = (RGBTRIPLE *)calloc((width + 2), sizeof(RGBTRIPLE));
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imagecopy[i + 1][j + 1].rgbtBlue = image[i][j].rgbtBlue;
            imagecopy[i + 1][j + 1].rgbtRed = image[i][j].rgbtRed;
            imagecopy[i + 1][j + 1].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            long long int gxBlue = ((-1) * imagecopy[i][j].rgbtBlue) + ((-2) * imagecopy[i + 1][j].rgbtBlue) + ((-1) * imagecopy[i + 2][j].rgbtBlue)
                                 + (imagecopy[i][j + 2].rgbtBlue) + (2 * imagecopy[i + 1][j + 2].rgbtBlue) + (imagecopy[i + 2][j + 2].rgbtBlue);
            long long int gyBlue = ((-1) * imagecopy[i][j].rgbtBlue) + ((-2) * imagecopy[i][j + 1].rgbtBlue) + ((-1) * imagecopy[i][j + 2].rgbtBlue)
                                 + (imagecopy[i + 2][j].rgbtBlue) + (2 * imagecopy[i + 2][j + 1].rgbtBlue) + (imagecopy[i + 2][j + 2].rgbtBlue);

            long long int gxRed = ((-1) * imagecopy[i][j].rgbtRed) + ((-2) * imagecopy[i + 1][j].rgbtRed) + ((-1) * imagecopy[i + 2][j].rgbtRed)
                                + (imagecopy[i][j + 2].rgbtRed) + (2 * imagecopy[i + 1][j + 2].rgbtRed) + (imagecopy[i + 2][j + 2].rgbtRed);
            long long int gyRed = ((-1) * imagecopy[i][j].rgbtRed) + ((-2) * imagecopy[i][j + 1].rgbtRed) + ((-1) * imagecopy[i][j + 2].rgbtRed)
                                + (imagecopy[i + 2][j].rgbtRed) + (2 * imagecopy[i + 2][j + 1].rgbtRed) + (imagecopy[i + 2][j + 2].rgbtRed);

            long long int gxGreen = ((-1) * imagecopy[i][j].rgbtGreen) + ((-2) * imagecopy[i + 1][j].rgbtGreen) + ((-1) * imagecopy[i + 2][j].rgbtGreen)
                                  + (imagecopy[i][j + 2].rgbtGreen) + (2 * imagecopy[i + 1][j + 2].rgbtGreen) + (imagecopy[i + 2][j + 2].rgbtGreen);
            long long int gyGreen = ((-1) * imagecopy[i][j].rgbtGreen) + ((-2) * imagecopy[i][j + 1].rgbtGreen) + ((-1) * imagecopy[i][j + 2].rgbtGreen)
                                  + (imagecopy[i + 2][j].rgbtGreen) + (2 * imagecopy[i + 2][j + 1].rgbtGreen) + (imagecopy[i + 2][j + 2].rgbtGreen);

            long double gtotalBlue = sqrt(gxBlue * gxBlue + gyBlue * gyBlue);
            long double gtotalRed = sqrt(gxRed * gxRed + gyRed * gyRed);
            long double gtotalGreen = sqrt(gxGreen * gxGreen + gyGreen * gyGreen);

            image[i][j].rgbtBlue = round((gtotalBlue > 255) ? 255 : gtotalBlue);
            image[i][j].rgbtRed = round((gtotalRed > 255) ? 255 : gtotalRed);
            image[i][j].rgbtGreen = round((gtotalGreen > 255) ? 255 : gtotalGreen);
        }
    }

    for (int i = 0; i < height + 2; i++)
    {
        if (imagecopy[i])
        {
            free(imagecopy[i]);
        }
    }

    if (imagecopy)
    {
        free(imagecopy);
    }

    return;
}
