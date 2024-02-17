# TODO
from cs50 import get_string


def main():
    card = get_string("Number: ")
    cardL = len(card)
    if cardL == 13 or cardL == 15 or cardL == 16:
        if check(card) == 0:
            num1 = int(card[0])
            num2 = int(card[1])
            if num1 == 3 and (num2 == 4 or num2 == 7):
                print("AMEX")
            elif num1 == 5 and (
                num2 == 1 or num2 == 2 or num2 == 3 or num2 == 4 or num2 == 5
            ):
                print("MASTERCARD")
            elif num1 == 4:
                print("VISA")
            else:
                print("INVALID")
        else:
            print("INVALID")
    else:
        print("INVALID")


def check(number):
    sum1 = 0
    sum2 = 0
    numberL = len(number)

    for count in range(numberL):
        index = (numberL - 1) - count
        digit = int(number[index])

        if not (count % 2 == 0):
            new = digit * 2

            if new > 9:
                new = str(new)
                sum2 = sum2 + int(new[0]) + int(new[1])
            else:
                sum2 = sum2 + new

        else:
            sum1 = sum1 + digit

    total = sum1 + sum2
    totalstr = str(total)
    lastdigit = int(totalstr[len(totalstr) - 1])

    return lastdigit


if __name__ == "__main__":
    main()
