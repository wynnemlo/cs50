/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */

bool search(int value, int values[], int n)
{
    int beginning = 0;
    int ending = n - 1;
    
    while (ending >= beginning)
    {
        int middle = (beginning + ending) / 2;
        if (values[middle] == value)
        {
            return true;
        }
        else if (values[middle] > value)
        {
            ending = middle - 1;
        }
        else
        {
            beginning = middle + 1;
        }
    }
    return false;
}


/**
 * Sorts array of n values.
 */

void sort(int values[], int n)
{
    while (n > 0)
    {
    	for (int i = 0; i < n - 1; i++)
        {
            if (values[i] > values[i+1])
            {
                int temp = values[i+1];
                values[i+1] = values[i];
                values[i] = temp;
            }
        }
        n--;
        
    }
    return;
}

