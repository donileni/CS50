from cs50 import get_int


def main():

    while True:
        card = get_int("Number: ")
        string_card = str(card)
        length = len(string_card)

        if length == 13 or length == 15 or length == 16:
            break
        else:
            print("INVALID")
            exit()

    valid = valid_card(card, length)
    if (valid):
        first = string_card[0]
        second = string_card[1]

        string_number = first + second
        number = int(string_number)
        first_number = int(first)

        if (number == 34 or number == 37):
            print("AMEX")
        elif (number > 50 and number < 56):
            print("MASTERCARD")
        elif (first_number == 4):
            print("VISA")
        else:
            print("INVALID")
    else:
        print("INVALID")


def valid_card(card, length):
    sum_of_multiplied = 0
    sum_of_remaining = 0
    for i in range(length):
        if (i % 2) == 0:
            n = get_digit(card, i)
            sum_of_remaining += n
        else:
            n = get_digit(card, i)
            n = n * 2
            if (n < 10):
                sum_of_multiplied += n
            else:
                string_n = str(n)
                n_length = len(string_n)
                for i in range(n_length):
                    m = get_digit(n, i)
                    sum_of_multiplied += m

    total_sum = sum_of_multiplied + sum_of_remaining

    if get_digit(total_sum, 0) == 0:
        return True
    else:
        return False


def get_digit(number, n):
    return number // 10**n % 10


main()