#include <cs50.h>
#include <stdio.h>

int digitplace(long number, int dig_no);
int numbersize(long number);


int main(void)
{

    long number = get_long("Please Enter your Card Number: ");
    int sum1 = 0;
    int sum2 = 0;
    int size = numbersize(number);

    // the counter (i) will be used to identify when to stop the loop
    // the counter (j) will be used for digitplace for even numbers
    // the counter (k) will be used to digitplace for odd numbers
    // x will represent every other number from the last number x 2
    // y will represent the first digit of the product of x, z will represent the second digit
    // l will represent every other number from the last number

    for (int i = 0, j = 2, k = 1 ; i < size ; i++, j += 2, k += 2)
    {
        int x, y, z, l;
        x = digitplace(number, j) * 2;
        y = digitplace(x, 1);
        z = digitplace(x, 2);
        sum1 += y + z;

        l = digitplace(number, k);
        sum2 += l;
    }

    // lastTwoDigits will represent the number before the last number + the last number x 10 to get the last two digits
    // digitplace (verify, 1) is to verify the first digit of the sum either its 0 or not

    int verify = sum1 + sum2;
    int lastTwoDigits = digitplace(number, size - 1) + digitplace(number, size) * 10;
    if (digitplace(verify, 1) == 0)
    {
        if ((size == 15) && ((lastTwoDigits == 34 | lastTwoDigits == 37)))
        {
            printf("AMEX\n");
        }
        else if (size >= 13 && size <= 16 && digitplace(number, size) == 4)
        {
            printf("VISA\n");
        }
        else if ((size == 16) && (lastTwoDigits >= 51 && lastTwoDigits <= 55))
        {
            printf("MASTERCARD\n");
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





// function to get a required digit from a number
// the function divides the number by 10 and iterates this process until it reaches the digit number required
// when it reaches it we get the remainder of the number when divided by 10 to get that digit then returns it

int digitplace(long number, int dig_no)
{
    int digit;
    for (int i = 0 ; i < dig_no ; i++)
    {
        digit = number % 10;
        number /= 10;
    }
    return digit;
}


// function to count the number of digits
// we can remove the last digit of the number by diving number on 10
// i will increment for each iteration of that division
// until the division equals zero so there is no digits left
// we make do-while, not while, to make i = 1 even if the number entered was 0

int numbersize(long number)
{
    int i = 0;
    do
    {
        number /= 10;
        i = i + 1;
    }
    while (number != 0);
    return i;
}
