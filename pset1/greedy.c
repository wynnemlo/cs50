/**
 * greedy.c
 *
 * Wynne Lo
 * wynnemlo@hku.hk
 *
 * Returns the number of coins that is 
 * needed for a user-specified amount of change,
 * using the greedy algorithm.
 */

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change;
    int coins = 0;
    
    // asks user for the amount of change (must be positive value)
    do
    {
        printf("O hai! How much change is owed?\n");
        change = GetFloat();
    }
    while (change < 0.00);
    
    // convert change's amount to number of cents
    int cents = round(change * 100);
    
    // prints the number of coins used for change
    while (cents > 0)
    {
        if (cents >= 25)
        {
            coins = coins + cents / 25;
            cents = cents % 25;
        }
        if (cents >= 10)
        {
            coins = coins + cents / 10;
            cents = cents % 10;
        }
        if (cents >= 5)
        {
            coins = coins + cents / 5;
            cents = cents % 5;
        }
        if (cents >= 1)
        {
            coins = coins + cents / 1;
            cents = 0;
        }
    }
    printf("%i\n", coins);
}
