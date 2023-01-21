#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>



int main(int argc, string argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    int key_length = strlen(argv[1]);

    if (key_length != 26)
    {
        printf("Key must be 26 characters.\n");
        return 1;
    }

    for (int i = 0; i < key_length; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("All key values must be aplhabetical\n");
            return 1;
        }
        for (int j = i + 1; j < key_length; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key should not contain repeated characters\n");
                return 1;
            }
        }
    }

    string key = argv[1];
    string user_input = get_string("plaintext:  ");
    printf("ciphertext: ");

    for (int i = 0; i < strlen(user_input); i++)
    {


        char c = user_input[i];
        int n = (int) c;
        int number = 0;

        if (isalpha(c))
        {
            if (isupper(c))
            {
                number = n - 65;
                c = key[number];
                c = toupper(c);
                printf("%c", c);
            }
            else
            {
                number = n - 97;
                c = key[number];
                c = tolower(c);
                printf("%c", c);
            }
        }
        else
        {
            printf("%c", c);
        }

    }

    printf("\n");
    return 0;
}