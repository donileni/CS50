#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{

    string input = get_string("Text: ");

    int letters = count_letters(input);
    int words = count_words(input);
    int sentences = count_sentences(input);


    //int S = ((float) sentences / (float) words) * 100;
    //int L = ((float) letters / (float) words) * 100;

    double L = (double) letters / words * 100;
    double S = (double) sentences / words * 100;

    int index = round(((0.0588 * L) - (0.296 * S) - 15.8));

    //printf("%s\n", input);

    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }


}

int count_letters(string text)
{
    int number_of_letters = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        char c = text[i];
        char upper = toupper(c);
        int n = (int) upper;

        if (upper >= 65 && upper <= 90)
        {
            number_of_letters ++;
        }
    }

    return number_of_letters;
}

int count_words(string text)
{
    int number_of_words = 1;

    for (int i = 0; i < strlen(text); i++)
    {
        char c = text[i];
        int n = (int) c;
        if (n == 32)
        {
            number_of_words++;
        }
    }
    return number_of_words;
}

int count_sentences(string text)
{
    int number_of_sentences = 0;

    for (int i = 0; i < strlen(text); i++)
    {
        char c = text[i];
        int n = (int) c;
        if (n == 33 || n == 46 || n == 63)
        { 
            number_of_sentences++;
        }
    }

    return number_of_sentences;
}