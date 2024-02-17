#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int x;

    // to keep asking for input if not from 1 to 8
    while (true)
    {
        x = get_int("Please Enter the Height:");
        
        if (x <= 8 && x > 0)
        {
            break;
        }
    }

    // for loop only to indentify "i" as row number
    for (int i = 1; i <= x; i++)
    {
        // to insert spaces decreasing in number with increasing the row number
        for (int j = 1; j <= x - i; j++)
        {
            printf(" ");
        }

        // to insert # increasing in number for each row
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }

        //the gap
        printf("  ");

        // to insert "#" increasing in number for each row after the gap
        for (int l = 1; l <= i; l++)
        {
            printf("#");
        }

        //to end the line after each row
        printf("\n");
    }
}
