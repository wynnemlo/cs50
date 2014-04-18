/**
 * mario.c
 *
 * Wynne Lo
 * wynnemlo@hku.hk
 *
 * Returns a text Mario pyramid of a height between 0 and 23.
 */

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    
    // asks user for height of an integer between 0 and 23
    do
    {
        printf("Height: ");
        height = GetInt();
    }
    while (height < 0 || height > 23);

    
    int pymid_width = 2;
    int empty_width = height;
    
    
    // prints pyramid according to user-given height
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < empty_width - 1; j++) // print spaces
        {
            printf(" ");
        }
        empty_width = empty_width - 1;
        for (int k = 0; k < pymid_width; k++) // print hashes
        {
            printf("#");
        } 
        pymid_width = pymid_width + 1;
        printf("\n"); // print new line
    }
}
