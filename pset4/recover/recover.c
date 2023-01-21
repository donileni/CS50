#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *output = NULL;

    if (input == NULL)
    {
        printf("Could not read file\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    char fileName[8];
    int counter = 0;

    while (fread(&buffer, BLOCK_SIZE, 1, input))
    {
        if (buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF && (buffer[3] & 0xF0) == 0xE0)
        {
            if (counter > 0)
            {
                fclose(output);
            }

            sprintf(fileName, "%03d.jpg", counter);

            output = fopen(fileName, "w");
            if (output == NULL)
            {
                return 1;
            }

            counter++;
        }

        if (output != NULL)
        {
            fwrite(&buffer, BLOCK_SIZE, 1, output);
        }
    }
    fclose(output);
    fclose(input);
}