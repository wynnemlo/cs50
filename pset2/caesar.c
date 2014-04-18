/**
 * caesar.c
 *
 * Wynne Lo
 * wynnemlo@hku.hk
 *
 * Encipher text with Caesar cipher.
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    // confirm that user inputted an integer in cmd-line
    if (argc != 2)
    {
        printf("Please provide a valid integer value.\n");
        return 1;
    }
    else 
    {
        // convert inputted cmd-line string to integer
        int k = atoi(argv[1]);
        // check if cmd-line string is a valid integer
        if (k == 0)
        {
            printf("Please provide a valid integer value.\n");
            return 1;
        }
        else
        {
            // ask for plain text input
            string p = GetString();
            // print ciphered text
            for (int i = 0, n = strlen(p); i < n; i++)
            {
                // check if char is an alphabet
                if (isalpha(p[i]))
                {
                    // check if char is uppercase
                    if (isupper(p[i])) 
                    {
                        int alphaI = p[i] - 65;
                        int newalphaI = (alphaI + k) % 26;
                        int newasciiI = newalphaI + 65;
                        printf("%c", newasciiI);           
                    }
                    // char is lowercase
                    else 
                    {
                        int alphaI = p[i] - 97; 
                        int newalphaI = (alphaI + k) % 26; 
                        int newasciiI = newalphaI + 97;
                        printf("%c", newasciiI);
                    }
                }
                else
                {
                    printf("%c", p[i]);
                }
            }
            printf("\n");
        }
        return 0;
    }
}
