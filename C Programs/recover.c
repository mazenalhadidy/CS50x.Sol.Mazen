#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Please ONLY enter the file name.");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");

    if (inptr == NULL)
    {
        printf("Please enter a valid file.");
        return 1;
    }

    typedef uint8_t BYTE;

    int i = 0, j = 0, k = 0, c = 0;
    char outfile[8];

    FILE *outptr = NULL;
    BYTE *buffer = malloc(512 * (sizeof(BYTE)));

    while (fread(buffer, 1, 512, inptr) == 512)
    {
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff) && ((buffer[3] & 0xf0) == 0xe0))
        {
            if (c == 1)
            {
                fclose(outptr);
            }
            sprintf(outfile, "%d%d%d.jpg", i, j, k);

            outptr = fopen(outfile, "a");
            fwrite(buffer, 1, 512, outptr);

            k++;
            if (k == 10)
            {
                k = 0;
                j++;
            }
            if (j == 10)
            {
                j = 0;
                i++;
            }
            if (i == 10)
            {
                i = 0;
            }

            c = 1;
        }

        else
        {
            if (c == 1)
            {
                fwrite(buffer, 1, 512, outptr);
            }
        }
    }

    free(buffer);
    fclose(outptr);
    fclose(inptr);
}