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
    // confirm that user inputted a string in cmd-line
    if (argc != 2)
    {
        printf("Please provide a valid string value.\n");
        return 1;
    }
    else
    {        
        string k = argv[1];
        for (int i = 0, n = strlen(k); i < n; i++)
        {
            if (!isalpha(k[i]))
            {
                printf("Please provide plaintext as key.\n");
                return 1;
            }
        }
        // ask user for plain text input
        string p = GetString();
        // declare variables
        int keypos = 0;
        int klen = strlen(k);
        // encipher input
        for (int i = 0, n = strlen(p); i < n; i++)
        {
            // convert key's char to alphabetical index
            int shift;
            if (isupper(k[keypos % klen]))
            {
                shift = (k[keypos % klen]) - 65;
            }
            else
            {
                shift = (k[keypos % klen]) - 97;
            }
            // check if p char is alphabetical
            if (isalpha(p[i]))
            {
                // check if p char is uppercase
                if (isupper(p[i]))
                {
                    int palpha = p[i] - 65;
                    int newASCII = ((palpha + shift) % 26) + 65;
                    printf("%c", newASCII);
                    keypos++;
                }
                // if p char is lowercase
                else
                {
                    int palpha = p[i] - 97;
                    int newASCII = ((palpha + shift) % 26) + 97;
                    printf("%c", newASCII);
                    keypos++;
                }
            }
            // if p char is non-alphabetical
            else
            {
                printf("%c", p[i]);
            }
        }
        printf("\n");
    }
}
