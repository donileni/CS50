#include <cs50.h>
#include <stdio.h>

void print_pyramid(int height);

int main(void)
{
    int height = 0;

    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    print_pyramid(height);

}

void print_pyramid(int height)
{
    for (int i = 0; i < height; i++) //rows
    {
        for (int j = 1; j <= height; j++) //collums
        {
            if (j < (height - i))
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }

        printf("  ");

        for (int k = 1; k <= height; k++) //collums
        {
            if (k < (2 + i))
            {
                printf("#");
            }
        }

        printf("\n");
    }
}