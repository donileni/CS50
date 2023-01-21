#include <cs50.h>
#include <stdio.h>

int get_long_length(long card_number);
int get_check_number(long card_number);
int check_number(int number);
int get_first_two_digits(float card_number);
int get_first_digit(float card_number);

int main(void)
{
    long card_number;

    do
    {
        card_number = get_long("Number: ");
    }
    while (card_number < 1);

    int card_number_lenght = get_long_length(card_number);
    int first_two = get_first_two_digits(card_number);
    int first = get_first_digit(card_number);
    int number = get_check_number(card_number);
    int checked_number = check_number(number);

    if (card_number_lenght != 13 && card_number_lenght != 15 && card_number_lenght != 16)
    {
        printf("INVALID\n");
    }
    else
    {
        if (checked_number)
        {
            if (first_two == 34 || first_two == 37)
            {
                printf("AMEX\n");
            }
            else if (first_two < 56 && first_two > 50)
            {
                printf("MASTERCARD\n");
            }
            else if (first == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }


        }
        else
        {
            printf("INVALID\n");
        }

    }

}

int get_long_length(long card_number)
{
    int number = 0;
    do
    {
        ++number;
        card_number = card_number / 10;
    }
    while (card_number);

    return number;
}

int get_check_number(long card_number)
{
    int sum1 = 0;
    int sum2 = 0;
    int total = 0;
    int mod1;
    int mod2;
    int d1;
    int d2;
    long number = card_number;

    do
    {
        mod1 = number % 10;
        number = number / 10;
        sum1 = sum1 + mod1;

        mod2 = number % 10;
        number = number / 10;

        mod2 = mod2 * 2;
        d1 = mod2 % 10;
        d2 = mod2 / 10;
        sum2 = sum2 + d1 + d2;
    }
    while (number > 0);

    total = sum1 + sum2;

    return total;
}

int check_number(int number)
{
    if (number % 10 != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}

int get_first_two_digits(float card_number)
{
    long number = card_number;

    do
    {
        number = number / 10;
    }
    while (number > 100);

    return number;
}

int get_first_digit(float card_number)
{
    long number = card_number;

    do
    {
        number = number / 10;
    }
    while (number > 10);

    return number;
}



