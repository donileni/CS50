from cs50 import get_string


def main():
    text = get_string("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    L = letters / words * 100
    S = sentences / words * 100

    index = round((0.0588 * L) - (0.296 * S) - 15.8)

    if index >= 16:
        print(f"Grade 16+")
    elif index < 1:
        print(f"Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(text):
    text_len = len(text)
    n = 0

    for i in range(text_len):
        if text[i].isalpha():
            n += 1

    return n


def count_words(text):
    text_len = len(text)
    n = 1

    for i in range(text_len):
        if text[i] == " ":
            n += 1

    return n


def count_sentences(text):
    text_len = len(text)
    n = 0

    for i in range(text_len):
        if text[i] == "." or text[i] == "!" or text[i] == "?":
            n += 1

    return n


main()