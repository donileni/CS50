from cs50 import get_int

input = 0
while input > 8 or input < 1:
    input = get_int("Height: ")

for i in range(1, input + 1):
    print(" " * (input - i) + ("#" * i) + "  " + ("#" * i))